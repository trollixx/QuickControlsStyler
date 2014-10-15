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
    void selectControl(int index);
    void updateModifiedMark(const QString &name, bool modified);

    void newStyle();
    void openStyle();

private: // Methods
    void addStyle(const Style &style, bool select = true);
    void findBuiltInStyles();
    void setupActions();
    static bool copyRecursively(const QString &srcFilePath, const QString &tgtFilePath);

private: // Variables
    Ui::MainWindow *ui;

    StylerQmlObject *m_qmlStyler = nullptr;

    QList<Style> m_styles;

    struct CodeCacheItem {
        int index;
        QString name;
        QString code;
        int cursorPosition = 0;
        int horizontalScrollPosition = 0;
        int verticalScrollPosition = 0;
        bool modified = false;
    };

    QHash <QString, CodeCacheItem> m_codeCache;
    QString m_currentControlName;
};

#endif // MAINWINDOW_H
