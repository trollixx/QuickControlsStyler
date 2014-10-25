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

#ifndef STYLE_H
#define STYLE_H

#include <QString>
#include <QStringList>

class Style
{
public:
    explicit Style(const QString &fullPath, bool builtIn = false);
    explicit Style(const QString &name, const QString &path, bool builtIn = false);

    QString name() const;
    void setName(const QString &name);

    QString path() const;
    void setPath(const QString &path);

    QString fullPath() const;

    bool isBuiltIn() const;
    void setBuiltIn(bool isBuiltIn);

    bool isReadOnly() const;
    void setReadOnly(bool isReadOnly);

    const QStringList &controls() const;
    QString controlFilePath(const QString &control) const;

private:
    QString m_name;
    QString m_path;
    bool m_builtIn = false;
    bool m_readOnly = false;

    QStringList m_controls;
};

inline bool operator==(const Style &lhs, const Style &rhs)
{
    return lhs.fullPath() == rhs.fullPath();
}

inline bool operator!=(const Style &lhs, const Style &rhs)
{
    return !(lhs == rhs);
}

#endif // STYLE_H
