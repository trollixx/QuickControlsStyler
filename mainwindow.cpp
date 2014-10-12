#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <newstyledialog.h>
#include "qmlsyntaxhighlighter.h"
#include "stylerqmlobject.h"

#include <QDir>
#include <QQmlContext>
#include <QQmlEngine>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_qmlStyler(new StylerQmlObject(this))
{
    ui->setupUi(this);
    setupActions();

    findBuiltInStyles();

    foreach (const Style &style, m_styles) {
        QString name = style.name();
        if (style.isBuiltIn())
            name += tr(" (built-in)");
        if (style.isReadOnly())
            name += tr(" (read only)");
        ui->styleComboBox->addItem(name);
    }

    connect(ui->controlComboBox, &QComboBox::currentTextChanged, this, &MainWindow::selectControl);
    connect(ui->styleComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &MainWindow::selectStyle);

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
    const Style &style = m_styles.at(index);
    qDebug("Selected style: %s", qPrintable(style.name()));

    ui->plainTextEdit->setReadOnly(style.isReadOnly());
    ui->plainTextEdit->clear();
    m_codeCache.clear();

    ui->controlComboBox->clear();
    foreach (const QString &control, style.controls())
        ui->controlComboBox->addItem(control, control);
    if (ui->controlComboBox->count())
        selectControl(0);

    m_qmlStyler->setStyleInfo(style.name(), style.path());
}

void MainWindow::selectControl(const QString &name)
{
    if (name.isEmpty())
        return;

    qDebug("Loading code for: %s", qPrintable(name));
    if (!m_codeCache.contains(name)) {
        const Style &style = m_styles.at(ui->styleComboBox->currentIndex());
        QScopedPointer<QFile> file(new QFile(style.controlFilePath(name)));
        if (!file->open(QIODevice::ReadOnly)) {
            qWarning("Cannot open file '%s': %s", qPrintable(file->fileName()),
                     qPrintable(file->errorString()));
            return;
        }
        m_codeCache.insert(name, file->readAll());
    }

    ui->plainTextEdit->setPlainText(m_codeCache.value(name));
}

void MainWindow::newStyle()
{
    QStringList styleNames;
    foreach (const Style &style, m_styles)
        styleNames << style.name();

    QScopedPointer<NewStyleDialog> dialog(new NewStyleDialog(styleNames, this));
    if (dialog->exec() == QDialog::Rejected)
        return;
}

void MainWindow::findBuiltInStyles()
{
    foreach (const QString &importPath, ui->quickWidget->engine()->importPathList()) {
        QDir dir(importPath + QStringLiteral("/QtQuick/Controls/Styles"));
        if (!dir.exists())
            continue;
        foreach (const QString &styleName, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
            qDebug("Built-in style found: %s", qPrintable(styleName));
            m_styles << Style(styleName, dir.absolutePath(), true);
        }
    }
}

void MainWindow::setupActions()
{
    connect(ui->actionNewStyle, &QAction::triggered, this, &MainWindow::newStyle);
    connect(ui->actionExit, &QAction::triggered, qApp, &QApplication::quit);

    connect(ui->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);
}
