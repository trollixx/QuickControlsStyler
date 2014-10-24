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

#ifndef STYLERQMLOBJECT_H
#define STYLERQMLOBJECT_H

#include <QObject>

class QQmlEngine;

class StylerQmlObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString styleName READ styleName NOTIFY styleInfoChanged)
    Q_PROPERTY(QString stylePath READ stylePath NOTIFY styleInfoChanged)
    Q_PROPERTY(QString currentControl READ currentControl NOTIFY currentControlChanged)
public:
    explicit StylerQmlObject(QQmlEngine *engine, QObject *parent = 0);

    void setStyleInfo(const QString &name, const QString &path);

    QString styleName() const;
    QString stylePath() const;

    Q_INVOKABLE void trimCache();

    QString currentControl() const;
    void setCurrentControl(const QString &currentControl);

signals:
    void styleInfoChanged();
    void currentControlChanged();

private:
    QString m_name;
    QString m_path;

    QString m_currentControl;

    QQmlEngine *m_engine;
};

#endif // STYLERQMLOBJECT_H
