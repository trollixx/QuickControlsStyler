#include "mainwindow.h"
#include "ui_mainwindow.h"

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

    ui->setupUi(this);

    foreach (const QString &control, m_controls)
        ui->controlComboBox->addItem(control);

    connect(ui->controlComboBox, &QComboBox::currentTextChanged, this, &MainWindow::chooseControl);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::chooseControl(const QString &name)
{
    Q_UNUSED(name)
}
