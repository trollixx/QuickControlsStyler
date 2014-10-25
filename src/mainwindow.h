/****************************************************************************
**
**  Copyright (C) 2014 Oleg Shparber <trollixx+github@gmail.com>
**
**  This file is part of QuickControlsStyler (QCStyler).
**  Web site: https://github.com/trollixx/QuickControlsStyler
**
**  QCStyler is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  QCStyler is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with QCStyler. If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "style.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QSettings;

class StylerQmlObject;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    struct CodeCacheItem {
        int index;
        QString name;
        QString filePath;
        QString code;
        int cursorPosition = 0;
        int horizontalScrollPosition = 0;
        int verticalScrollPosition = 0;
        bool modified = false;
    };

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void selectStyle(int index);
    void selectControl(int index);
    void updateModifiedMark(const QString &name, bool modified);

    void newStyle();
    void openStyle();
    void openRecentStyle();

    void saveFile();
    void saveAll();

private: // Methods
    void loadSettings();
    void saveSettings();

    void addRecentStyleMenuItem(const QString &path);
    void addStyle(const Style &style, bool select = true);
    void save(const QString &name);
    void reloadPreview();
    void findBuiltInStyles();
    void setupActions();

    static bool copyRecursively(const QString &srcFilePath, const QString &tgtFilePath);

private: // Variables
    Ui::MainWindow *ui;

    QSettings *m_settings = nullptr;
    QStringList m_recentStyles;

    StylerQmlObject *m_qmlStyler = nullptr;

    QList<Style> m_styles;

    QHash <QString, CodeCacheItem> m_codeCache;
    QString m_currentControlName;
};

#endif // MAINWINDOW_H
