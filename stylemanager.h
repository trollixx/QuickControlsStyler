#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H

#include <QHash>
#include <QObject>
#include <QStringList>

class QQmlEngine;

class StyleManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentStyleName READ currentStyleName NOTIFY currentStyleChanged)
    Q_PROPERTY(QString currentStylePath READ currentStylePath NOTIFY currentStyleChanged)
public:
    explicit StyleManager(QQmlEngine *qmlEngine, QObject *parent = 0);

    const QStringList &availableControls() const;
    const QStringList &availableStyles() const;

    QString currentStyleName() const;
    QString currentStylePath() const;

signals:
    void currentStyleChanged();

public slots:
    void addStyle(const QString &name, const QString &path);
    void loadStyle(const QString &name);

private:
    void findDefaultStyles();

    QQmlEngine *m_qmlEngine;

    QStringList m_controls;
    QStringList m_styles;
    QHash<QString, QString> m_stylePaths;

    QString m_currentControl;
    QString m_currentStyle;
};

#endif // STYLEMANAGER_H
