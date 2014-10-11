#include "stylemanager.h"

#include <QDir>
#include <QQmlEngine>

StyleManager::StyleManager(QQmlEngine *qmlEngine, QObject *parent) :
    QObject(parent),
    m_qmlEngine(qmlEngine)
{
    // Default controls
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

    findDefaultStyles();

    m_currentControl = m_controls.first();
    m_currentStyle = m_styles.first();
}

const QStringList &StyleManager::availableControls() const
{
    return m_controls;
}

const QStringList &StyleManager::availableStyles() const
{
    return m_styles;
}

QString StyleManager::currentStyleName() const
{
    return m_currentStyle;
}

QString StyleManager::currentStylePath() const
{
    return m_stylePaths.value(m_currentStyle);
}

void StyleManager::addStyle(const QString &name, const QString &path)
{
    m_styles << name;
    m_stylePaths.insert(name, path);
    loadStyle(name);
}

void StyleManager::loadStyle(const QString &name)
{
    if (name == m_currentStyle)
        return;

    m_currentStyle = name;
    emit currentStyleChanged();
}

void StyleManager::findDefaultStyles()
{
    // Default styles
    const QStringList defaultStyles = QStringList{"Base", "Desktop"};

    foreach (const QString &styleName, defaultStyles) {
        bool found = false;
        foreach (const QString &importPath, m_qmlEngine->importPathList()) {
            QDir dir(importPath + QStringLiteral("/QtQuick/Controls/Styles"));
            if (dir.exists(styleName)) {
                found = true;
                addStyle(styleName, dir.absolutePath());
                break;
            }
        }
        if (!found)
            addStyle(styleName, QStringLiteral("qrc:/QtQuick/Controls/Styles/"));
    }
}
