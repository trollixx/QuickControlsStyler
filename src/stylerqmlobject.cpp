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

QString StylerQmlObject::currentControl() const
{
    return m_currentControl;
}

void StylerQmlObject::setCurrentControl(const QString &currentControl)
{
    if (currentControl == m_currentControl)
        return;

    m_currentControl = currentControl;
    emit currentControlChanged();
}
