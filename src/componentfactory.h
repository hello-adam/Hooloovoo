#ifndef COMPONENTFACTORY_H
#define COMPONENTFACTORY_H

#include <QString>
#include <QSharedPointer>
#include <QtXml>

#include "component.h"

class ComponentFactory
{
public:
    ComponentFactory();

    Component* createComponent(GameObject* parentObject, QString name);
    Component* createComponent(GameObject* parentObject, const QDomElement & specs);

    QStringList availableComponents();
};

#endif // COMPONENTFACTORY_H
