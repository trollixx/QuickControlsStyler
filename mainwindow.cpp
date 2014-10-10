#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QQmlContext>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Add known controls
    m_controls = QStringList{
        "BusyIndicator",
        "Button",
        "Calendar",
        "CheckBox",
        "ComboBox",
        "MenuBar",
        "Menu",
        "ProgressBar",
        "RadioButton",
        "Slider",
        "SpinBox",
        "StatusBar",
        "Switch",
        "TextArea",
        "TextField",
        "ToolBar"
    };

    m_styles = QStringList{
        "Base",
        "Desktop"
    };

    ui->setupUi(this);

    foreach (const QString &control, m_controls)
        ui->controlComboBox->addItem(control);

    foreach (const QString &style, m_styles)
        ui->styleComboBox->addItem(style);

    connect(ui->controlComboBox, &QComboBox::currentTextChanged, this, &MainWindow::chooseControl);
    connect(ui->styleComboBox, &QComboBox::currentTextChanged, this, &MainWindow::selectStyle);
    selectStyle(m_styles.first());
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

void MainWindow::selectStyle(const QString &name)
{
    ui->quickWidget->rootContext()->setContextProperty("styleName", QVariant::fromValue(name));
}
