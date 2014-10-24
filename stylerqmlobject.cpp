#include "stylerqmlobject.h"

#include <QQmlEngine>

StylerQmlObject::StylerQmlObject(QQmlEngine *engine, QObject *parent) :
    QObject(parent),
    m_engine(engine)
{
}

void StylerQmlObject::setStyleInfo(const QString &name, const QString &path)
{
    m_name = name;
    m_path = path;
    emit styleInfoChanged();
}

QString StylerQmlObject::styleName() const
{
    return m_name;
}

QString StylerQmlObject::stylePath() const
{
    return m_path;
}

void StylerQmlObject::trimCache()
{
    m_engine->trimComponentCache();
}
