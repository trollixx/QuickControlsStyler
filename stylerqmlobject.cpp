#include "stylerqmlobject.h"

StylerQmlObject::StylerQmlObject(QObject *parent) :
    QObject(parent)
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
