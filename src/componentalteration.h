#ifndef COMPONENTALTERATION_H
#define COMPONENTALTERATION_H

#include <QObject>
#include <QtXml>
#include "componentfactory.h"

class ComponentAlteration : public QObject
{
    Q_OBJECT
public:
    explicit ComponentAlteration(QObject *parent = 0);

    enum AlterationType
    {
        SetValue, Add
    };

    bool alterComponent(Component* component);

    QDomElement serialize();
    bool deserialze(QDomElement & specs);

    QString getStringValue();
    void setStringValue(QString value);

private:
    QString m_componentName;
    QString m_componentTag;
    QString m_componentProperty;
    QVariant m_propertyValue;

signals:

public slots:

};

#endif // COMPONENTALTERATION_H
