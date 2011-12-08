#include "componentalteration.h"

ComponentAlteration::ComponentAlteration(QObject *parent) :
    QObject(parent)
{
}

bool alterComponent(Component* component)
{
    return true;
}

QDomElement serialize()
{
    return QDomElement();
}

bool deserialze(QDomElement & specs)
{
    return true;
}
