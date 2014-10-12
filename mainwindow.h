#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "style.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class StylerQmlObject;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void selectStyle(int index);
    void selectControl(const QString &name);

private: // Methods
    void findBuiltInStyles();
    void setupActions();

private: // Variables
    Ui::MainWindow *ui;

    StylerQmlObject *m_qmlStyler = nullptr;

    QList<Style> m_styles;

    QHash <QString, QString> m_codeCache;
};

#endif // MAINWINDOW_H
