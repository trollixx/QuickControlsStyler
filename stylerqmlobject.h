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
