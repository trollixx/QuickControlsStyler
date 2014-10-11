#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H

#include "style.h"

#include <QHash>
#include <QObject>
#include <QStringList>

class QQmlEngine;

class StyleManager : public QObject
{
    Q_OBJECT
public:
    explicit StyleManager(QQmlEngine *qmlEngine, QObject *parent = 0);

    const QList<Style> &availableStyles() const;

    const Style &style(int index) const;

public slots:
    void selectStyle(int index);

private:
    void findBuiltInStyles();

    QQmlEngine *m_qmlEngine;

    QList<Style> m_styles;
    int m_currentStyleIndex = -1;
};

#endif // STYLEMANAGER_H
