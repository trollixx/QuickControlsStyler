/****************************************************************************
**
**  Copyright (C) 2014 Oleg Shparber <trollixx+github@gmail.com>
**
**  This file is part of QuickControlsStyler (QCStyler).
**  Web site: https://github.com/trollixx/QuickControlsStyler
**
**  QCStyler is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  QCStyler is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with QCStyler. If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "newstyledialog.h"
#include "qmlsyntaxhighlighter.h"
#include "stylerqmlobject.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QQmlContext>
#include <QQmlEngine>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Centring splitter handle
    const int half = ui->splitter->width() / 2 - ui->splitter->handleWidth();
    ui->splitter->setSizes({half, half});
    ui->controlComboBox->setFocusProxy(ui->plainTextEdit);

    setupActions();

    // Create now to avoid crash when styles are loaded
    m_qmlStyler = new StylerQmlObject(ui->quickWidget->engine(), this);

    findBuiltInStyles();

    connect(ui->controlComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &MainWindow::selectControl);
    connect(ui->styleComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &MainWindow::selectStyle);
    connect(ui->plainTextEdit, &QPlainTextEdit::textChanged, [this]() {
        if (ui->plainTextEdit->document()->isModified())
            updateModifiedMark(m_currentControlName, true);
    });

    ui->plainTextEdit->document()->setDefaultFont(QFont(QStringLiteral("Monospace"), 10));
    new QMLSyntaxHighlighter(ui->plainTextEdit->document());

    if (ui->styleComboBox->count())
        selectStyle(0);

    ui->quickWidget->rootContext()
            ->setContextProperty(QStringLiteral("__qcStyler"), m_qmlStyler);

    ui->quickWidget->setSource(QStringLiteral("qrc:/preview/main.qml"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::selectStyle(int index)
{
    if (index < 0)
        return;

    const Style &style = m_styles.at(index);

    ui->plainTextEdit->setReadOnly(style.isReadOnly());
    ui->actionSaveFile->setEnabled(!style.isReadOnly());
    ui->actionSaveAll->setEnabled(!style.isReadOnly());
    m_codeCache.clear();

    ui->controlComboBox->clear();
    foreach (const QString &control, style.controls())
        ui->controlComboBox->addItem(control, control);

    m_qmlStyler->setStyleInfo(style.name(), style.path());
}

void MainWindow::selectControl(int index)
{
    if (index < 0)
        return;

    if (m_codeCache.contains(m_currentControlName)) {
        CodeCacheItem &cci = m_codeCache[m_currentControlName];
        cci.cursorPosition = ui->plainTextEdit->textCursor().position();
        cci.horizontalScrollPosition = ui->plainTextEdit->horizontalScrollBar()->value();
        cci.verticalScrollPosition = ui->plainTextEdit->verticalScrollBar()->value();
        if (ui->plainTextEdit->document()->isModified())
            cci.code = ui->plainTextEdit->toPlainText();
    }

    const QString name = ui->controlComboBox->itemData(index).toString();
    if (!m_codeCache.contains(name)) {
        const Style &style = m_styles.at(ui->styleComboBox->currentIndex());
        QScopedPointer<QFile> file(new QFile(style.controlFilePath(name)));
        if (!file->open(QIODevice::ReadOnly)) {
            /// TODO: MessageBox
            qWarning("Cannot open file '%s': %s", qPrintable(file->fileName()),
                     qPrintable(file->errorString()));
            return;
        }
        CodeCacheItem cci;
        cci.index = index;
        cci.name = name;
        cci.filePath = file->fileName();
        cci.code = file->readAll();
        m_codeCache.insert(name, cci);
    }

    m_currentControlName = name;

    const CodeCacheItem &cci = m_codeCache[name];

    // Avoid textChanged() signal, when another file is loaded
    ui->plainTextEdit->blockSignals(true);
    ui->plainTextEdit->setPlainText(cci.code);
    ui->plainTextEdit->blockSignals(false);

    QTextCursor textCursor = ui->plainTextEdit->textCursor();
    textCursor.setPosition(cci.cursorPosition);
    ui->plainTextEdit->setTextCursor(textCursor);

    ui->plainTextEdit->horizontalScrollBar()->setValue(cci.horizontalScrollPosition);
    ui->plainTextEdit->verticalScrollBar()->setValue(cci.verticalScrollPosition);

    m_qmlStyler->setCurrentControl(name);
}

/*!
  \internal
  Shows or hides a modified mark (asterisk) for a control specified with \a name,
  according to \a modified value.
*/
void MainWindow::updateModifiedMark(const QString &name, bool modified)
{
    CodeCacheItem &cci = m_codeCache[name];

    if (cci.modified == modified)
        return;

    cci.modified = modified;

    QString displayName = cci.name;

    if (modified)
        displayName += QStringLiteral(" *");

    ui->controlComboBox->setItemText(cci.index, displayName);

    if (cci.name == m_currentControlName)
        ui->plainTextEdit->document()->setModified(false);
}

void MainWindow::newStyle()
{
    QStringList styleNames;
    foreach (const Style &style, m_styles)
        styleNames << style.name();

    QScopedPointer<NewStyleDialog> dialog(new NewStyleDialog(styleNames, this));
    if (dialog->exec() == QDialog::Rejected)
        return;

    const Style &baseStyle = m_styles.at(dialog->baseStyleIndex());
    const QString destination = dialog->location() + QStringLiteral("/") + dialog->name();

    /// TODO: Copy only required files?
    if (!copyRecursively(baseStyle.fullPath(), destination)) {
        QMessageBox::warning(this, tr("Error"),
                             tr("Cannot create style in the specified location."));
        return;
    }

    addStyle(Style(dialog->name(), dialog->location()));
}

/*!
  \internal
  Opens directory as a style.
*/
void MainWindow::openStyle()
{
    const QFileInfo fi(QFileDialog::getExistingDirectory(this));
    if (!fi.exists())
        return;
    addStyle(Style(fi.fileName(), fi.absolutePath()));
}

/*!
  \internal
  Saves content of the current file.
*/
void MainWindow::saveFile()
{
    save(m_currentControlName);
    reloadPreview();
}

/*!
  \internal
  Saves contents of all modified files.
*/
void MainWindow::saveAll()
{
    foreach (const QString &name, m_codeCache.keys())
        save(name);
    reloadPreview();
}

/*!
  \internal
  Adds style to the style list. The added style will be selected if \a select is \c true.
*/
void MainWindow::addStyle(const Style &style, bool select)
{
    if (m_styles.contains(style)) {
        QMessageBox::information(this, tr("Error"), tr("Style is already open."));
        return;
    }

    m_styles.append(style);

    QString displayName = style.name();
    if (style.isBuiltIn())
        displayName += tr(" (built-in)");
    if (style.isReadOnly())
        displayName += tr(" (read only)");

    ui->styleComboBox->addItem(displayName);

    if (select)
        ui->styleComboBox->setCurrentIndex(ui->styleComboBox->count() - 1);
}

/*!
  \internal
  Actually performs file saving. Called by \l saveFile and \l saveAll.

  \sa saveFile, saveAll
*/
void MainWindow::save(const QString &name)
{
    CodeCacheItem &cci = m_codeCache[name];

    if (!cci.modified)
        return;

    QScopedPointer<QFile> file(new QFile(cci.filePath));
    if (!file->open(QIODevice::WriteOnly)) {
        /// TODO: MessageBox
        qWarning("Cannot open file '%s': %s", qPrintable(file->fileName()),
                 qPrintable(file->errorString()));
        return;
    }

    if (name == m_currentControlName)
        cci.code = ui->plainTextEdit->toPlainText();

    if (file->write(cci.code.toUtf8()) == -1) {
        /// TODO: MessageBox
        qWarning("Cannot write to file '%s': %s", qPrintable(file->fileName()),
                 qPrintable(file->errorString()));
        return;
    }

    updateModifiedMark(cci.name, false);
}

/*!
  \internal
*/
void MainWindow::reloadPreview()
{
    ui->quickWidget->engine()->clearComponentCache();
    /// TODO: Implement style reloading in QQuickStyleSettings
    ui->quickWidget->setSource(QStringLiteral("qrc:/preview/main.qml"));
}

/*!
  \internal
  Looks for styles built in Qt Quick Controls.
*/
void MainWindow::findBuiltInStyles()
{
    foreach (const QString &importPath, ui->quickWidget->engine()->importPathList()) {
        QDir dir(importPath + QStringLiteral("/QtQuick/Controls/Styles"));
        if (!dir.exists())
            continue;
        foreach (const QString &styleName, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
            addStyle(Style(styleName, dir.absolutePath(), true), false);
    }
}

void MainWindow::setupActions()
{
    connect(ui->actionNewStyle, &QAction::triggered, this, &MainWindow::newStyle);
    connect(ui->actionOpenStyle, &QAction::triggered, this, &MainWindow::openStyle);
    connect(ui->actionSaveFile, &QAction::triggered, this, &MainWindow::saveFile);
    connect(ui->actionSaveAll, &QAction::triggered, this, &MainWindow::saveAll);
    connect(ui->actionExit, &QAction::triggered, qApp, &QApplication::quit);

    connect(ui->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);
}

// Taken from Qt Creator (src/app/main.cpp)
bool MainWindow::copyRecursively(const QString &srcFilePath, const QString &tgtFilePath)
{
    QFileInfo srcFileInfo(srcFilePath);
    if (srcFileInfo.isDir()) {
        QDir targetDir(tgtFilePath);
        targetDir.cdUp();
        if (!targetDir.mkdir(QFileInfo(tgtFilePath).fileName()))
            return false;
        QDir sourceDir(srcFilePath);
        QStringList fileNames = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
        foreach (const QString &fileName, fileNames) {
            const QString newSrcFilePath = srcFilePath + QStringLiteral("/") + fileName;
            const QString newTgtFilePath = tgtFilePath + QStringLiteral("/") + fileName;
            if (!copyRecursively(newSrcFilePath, newTgtFilePath))
                return false;
        }
    } else {
        if (!QFile::copy(srcFilePath, tgtFilePath))
            return false;
    }
    return true;
}
