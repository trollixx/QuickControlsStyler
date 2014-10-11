#include "stylemanager.h"

#include <QDir>
#include <QQmlEngine>

StyleManager::StyleManager(QQmlEngine *qmlEngine, QObject *parent) :
    QObject(parent),
    m_qmlEngine(qmlEngine)
{
    findBuiltInStyles();
    if (!m_styles.isEmpty())
        m_currentStyleIndex = 0;
}

const QList<Style> &StyleManager::availableStyles() const
{
    return m_styles;
}

const Style &StyleManager::style(int index) const
{
    return m_styles.at(index);
}

void StyleManager::selectStyle(int index)
{
    if (index == m_currentStyleIndex)
        return;

    m_currentStyleIndex = index;
}

void StyleManager::findBuiltInStyles()
{
    foreach (const QString &importPath, m_qmlEngine->importPathList()) {
        QDir dir(importPath + QStringLiteral("/QtQuick/Controls/Styles"));
        if (!dir.exists())
            continue;
        foreach (const QString &styleName, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
            qDebug("%s", qPrintable(styleName));
            m_styles << Style(styleName, dir.absolutePath(), true);
        }
    }
}
