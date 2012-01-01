#include "componentalteration.h"

ComponentAlteration::ComponentAlteration(QObject *parent) :
    QObject(parent)
{
}

bool ComponentAlteration::alterComponent(Component* component)
{
    return true;
}

QString m_componentName;
QString m_componentTag;
QString m_componentProperty;
QVariant m_propertyValue;

QDomElement ComponentAlteration::serialize()
{
    QDomDocument doc;
    QDomElement data = doc.createElement("componentalteration");

    data.setAttribute("name", m_componentName);
    data.setAttribute("tag", m_componentTag);
    data.setAttribute("property", m_componentProperty);

    QDomElement value = doc.createElement("propertyvalue");

    return data;
}

bool ComponentAlteration::deserialze(QDomElement & specs)
{
    return true;
}

QString ComponentAlteration::getStringValue()
{
    return "";
}

void ComponentAlteration::setStringValue(QString value)
{

}
