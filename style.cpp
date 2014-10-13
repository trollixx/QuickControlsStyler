#include "style.h"

#include <QDir>
#include <QFileInfo>

Style::Style(const QString &name, const QString &path, bool builtIn) :
    m_name(name),
    m_path(path),
    m_builtIn(builtIn)
{
    QDir dir(m_path + QStringLiteral("/") + m_name);

    /// TODO: Inform about error
    if (!dir.exists()) {
        qWarning("Style cannot be loaded: Directory '%s' does not exist.", qPrintable(dir.path()));
        return;
    }

    // Built-in styles are read-only by default
    m_readOnly = m_builtIn ?  true : !QFileInfo(dir.absolutePath()).isWritable();

    const QString styleSuffix = QStringLiteral("*Style.qml");
    foreach (const QString &fileName, dir.entryList({styleSuffix}, QDir::Files | QDir::Readable))
        m_controls << fileName.left(fileName.length() - styleSuffix.length() + 1);
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
