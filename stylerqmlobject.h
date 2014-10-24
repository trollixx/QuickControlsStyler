#ifndef STYLERQMLOBJECT_H
#define STYLERQMLOBJECT_H

#include <QObject>

class QQmlEngine;

class StylerQmlObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString styleName READ styleName NOTIFY styleInfoChanged)
    Q_PROPERTY(QString stylePath READ stylePath NOTIFY styleInfoChanged)
public:
    explicit StylerQmlObject(QQmlEngine *engine, QObject *parent = 0);

    void setStyleInfo(const QString &name, const QString &path);

    QString styleName() const;
    QString stylePath() const;

    Q_INVOKABLE void trimCache();

signals:
    void styleInfoChanged();

private:
    QString m_name;
    QString m_path;

    QQmlEngine *m_engine;
};

#endif // STYLERQMLOBJECT_H
