#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "stylemanager.h"

#include <QQmlContext>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_styleManager = new StyleManager(ui->quickWidget->engine(), this);

    foreach (const QString &control, m_styleManager->availableControls())
        ui->controlComboBox->addItem(control);

    foreach (const QString &style, m_styleManager->availableStyles())
        ui->styleComboBox->addItem(style);

    //connect(ui->controlComboBox, &QComboBox::currentTextChanged, this, &MainWindow::chooseControl);
    connect(ui->styleComboBox, &QComboBox::currentTextChanged,
            m_styleManager, &StyleManager::loadStyle);
    ui->quickWidget->rootContext()
            ->setContextProperty(QStringLiteral("StyleManager"), m_styleManager);
    ui->quickWidget->setSource(QStringLiteral("qrc:/main.qml"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::chooseControl(const QString &name)
{
    Q_UNUSED(name)
}
