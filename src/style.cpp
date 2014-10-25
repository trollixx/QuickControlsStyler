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

#include "style.h"

#include <QDir>
#include <QFileInfo>

Style::Style(const QString &fullPath, bool builtIn) :
    m_builtIn(builtIn)
{
    QDir dir(fullPath);

    m_name = dir.dirName();
    m_path = dir.absolutePath();
    m_path.chop(m_name.size() + 1);

    /// TODO: Inform about error
    if (!dir.exists()) {
        qWarning("Style cannot be loaded: Directory '%s' does not exist.", qPrintable(dir.path()));
        return;
    }

    // Built-in styles are read-only by default
    m_readOnly = m_builtIn ?  true : !QFileInfo(dir.absolutePath()).isWritable();

    /// TODO: Move to Settings
    /// TODO: Implement preview
    static QStringList ignoredControls = {
        QStringLiteral("ApplicationWindow"),
        QStringLiteral("FocusFrame"),
        QStringLiteral("GroupBox"),
        QStringLiteral("MenuBar"),
        QStringLiteral("Menu")
    };

    const QString styleSuffix = QStringLiteral("*Style.qml");
    foreach (const QString &fileName, dir.entryList({styleSuffix}, QDir::Files | QDir::Readable)) {
        const QString name = fileName.left(fileName.length() - styleSuffix.length() + 1);
        if (ignoredControls.contains(name))
            continue;
        m_controls << name;
    }
}

Style::Style(const QString &name, const QString &path, bool builtIn) :
    Style(path + QStringLiteral("/") + name, builtIn)
{
}

QString Style::name() const
{
    return m_name;
}

void Style::setName(const QString &name)
{
    m_name = name;
}

QString Style::path() const
{
    return m_path;
}

void Style::setPath(const QString &path)
{
    m_path = path;
}

QString Style::fullPath() const
{
    return m_path + QStringLiteral("/") + m_name;
}

bool Style::isBuiltIn() const
{
    return m_builtIn;
}

void Style::setBuiltIn(bool builtIn)
{
    m_builtIn = builtIn;
}

bool Style::isReadOnly() const
{
    return m_readOnly;
}

void Style::setReadOnly(bool readOnly)
{
    m_readOnly = readOnly;
}

const QStringList &Style::controls() const
{
    return m_controls;
}

QString Style::controlFilePath(const QString &control) const
{
    return QString("%1/%2/%3Style.qml").arg(m_path).arg(m_name).arg(control);
}
