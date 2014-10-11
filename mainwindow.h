#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class StyleManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void selectStyle(int index);
    void selectControl(const QString &name);

private:
    Ui::MainWindow *ui;
    StyleManager *m_styleManager = nullptr;

    QHash <QString, QString> m_codeCache;
};

#endif // MAINWINDOW_H
