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
    ui(new Ui::MainWindow),
    m_qmlStyler(new StylerQmlObject(this))
{
    ui->setupUi(this);
    setupActions();

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
    ui->quickWidget->setSource(QStringLiteral("qrc:/main.qml"));
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
    qDebug("Selected style: %s", qPrintable(style.name()));

    ui->plainTextEdit->setReadOnly(style.isReadOnly());
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
    qDebug("Loading code for: %s", qPrintable(name));
    if (!m_codeCache.contains(name)) {
        const Style &style = m_styles.at(ui->styleComboBox->currentIndex());
        QScopedPointer<QFile> file(new QFile(style.controlFilePath(name)));
        if (!file->open(QIODevice::ReadOnly)) {
            qWarning("Cannot open file '%s': %s", qPrintable(file->fileName()),
                     qPrintable(file->errorString()));
            return;
        }
        CodeCacheItem cci;
        cci.index = index;
        cci.name = name;
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
}

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

void MainWindow::openStyle()
{
    const QFileInfo fi(QFileDialog::getExistingDirectory(this));
    if (!fi.exists())
        return;
    addStyle(Style(fi.fileName(), fi.absolutePath()));
}
void MainWindow::addStyle(const Style &style, bool select)
{
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

void MainWindow::findBuiltInStyles()
{
    foreach (const QString &importPath, ui->quickWidget->engine()->importPathList()) {
        QDir dir(importPath + QStringLiteral("/QtQuick/Controls/Styles"));
        if (!dir.exists())
            continue;
        foreach (const QString &styleName, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
            qDebug("Built-in style found: %s", qPrintable(styleName));
            addStyle(Style(styleName, dir.absolutePath(), true), false);
        }
    }
}

void MainWindow::setupActions()
{
    connect(ui->actionNewStyle, &QAction::triggered, this, &MainWindow::newStyle);
    connect(ui->actionOpenStyle, &QAction::triggered, this, &MainWindow::openStyle);
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
