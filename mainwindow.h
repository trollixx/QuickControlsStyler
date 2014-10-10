#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void chooseControl(const QString &name);
    void selectStyle(const QString &name);

private:
    Ui::MainWindow *ui;
    QStringList m_controls;
    QStringList m_styles;
};

#endif // MAINWINDOW_H
