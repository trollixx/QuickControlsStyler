#ifndef STYLE_H
#define STYLE_H

#include <QString>
#include <QStringList>

class Style
{
public:
    explicit Style(const QString &name, const QString &path, bool builtIn = false);

    QString name() const;
    void setName(const QString &name);

    QString path() const;
    void setPath(const QString &path);

    bool isBuiltIn() const;
    void setBuiltIn(bool isBuiltIn);

    bool isReadOnly() const;
    void setReadOnly(bool isReadOnly);

    const QStringList &controls() const;
    QString controlFilePath(const QString &control) const;

private:
    friend class StyleManager;

    QString m_name;
    QString m_path;
    bool m_builtIn = false;
    bool m_readOnly = false;

    QStringList m_controls;
};

#endif // STYLE_H
