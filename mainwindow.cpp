#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qmlsyntaxhighlighter.h"
#include "stylemanager.h"

#include <QQmlContext>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_styleManager = new StyleManager(ui->quickWidget->engine(), this);

    foreach (const Style &style, m_styleManager->availableStyles()) {
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

    ui->quickWidget->rootContext()
            ->setContextProperty(QStringLiteral("StyleManager"), m_styleManager);
    ui->quickWidget->setSource(QStringLiteral("qrc:/main.qml"));

    if (ui->styleComboBox->count())
        selectStyle(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::selectStyle(int index)
{
    /// TODO: Remove StyleManager::selectStyle()
    m_styleManager->selectStyle(index);

    const Style &style = m_styleManager->style(index);
    qDebug("Selected style: %s", qPrintable(style.name()));

    ui->plainTextEdit->setReadOnly(style.isReadOnly());
    ui->plainTextEdit->clear();
    m_codeCache.clear();

    ui->controlComboBox->clear();
    foreach (const QString &control, style.controls())
        ui->controlComboBox->addItem(control, control);
    if (ui->controlComboBox->count())
        selectControl(0);
}

void MainWindow::selectControl(const QString &name)
{
    if (name.isEmpty())
        return;

    qDebug("Loading code for: %s", qPrintable(name));
    if (!m_codeCache.contains(name)) {
        const Style &style = m_styleManager->style(ui->styleComboBox->currentIndex());
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
