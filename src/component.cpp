#include "component.h"
#include "gameobject.h"

Component::Component(GameObject *parentObject) :
    //QObject(parentObject),
    m_parentObject(parentObject)
{
    m_tag = "";

    if (parentObject == 0)
        m_ID = 0;
    else
        m_ID = parentObject->getAvailableComponentID();

    m_propertyRoot = 0;

    m_properties << new Property(this, "tag");
}

Component::~Component()
{
    qDeleteAll(m_properties);
    if (!m_propertyRoot)
        delete m_propertyRoot;
}

Property* Component::getPropertyRoot()
{
    if (!m_propertyRoot)
        m_propertyRoot = new Property(0, objectName());

    return m_propertyRoot;
}

GameObject* Component::getParentObject()
{
    if (m_parentObject)
        return m_parentObject;
    else if (qobject_cast<GameObject*>(this))
        return qobject_cast<GameObject*>(this);
    else
        return 0;
}

QDomElement Component::serialize()
{
    qDebug() << "serialize start";
    QDomDocument doc;

    this->prepareForSerialization();

    QDomElement componentElement = doc.createElement("component");

    componentElement.setAttribute("name", this->objectName());
    componentElement.setAttribute("id", m_ID);

    for (int i=0; i<m_properties.count(); i++)
    {
        componentElement.appendChild(m_properties.at(i)->serialize());
    }

    privateSerialize(componentElement);

    return componentElement;
}

bool Component::deserialize(const QDomElement &componentElement)
{
    m_ID = componentElement.attribute("id", "-1").toInt();
    if (m_ID == -1 && m_parentObject)
    {
        m_ID = m_parentObject->getAvailableComponentID();
    }
    else if (!m_parentObject)
    {
        m_ID = 0;
    }

    for (int i = 0; i<this->metaObject()->propertyCount(); i++)
    {
        QMetaProperty property = this->metaObject()->property(i);
        QString name = property.name();
        QDomElement prop = componentElement.firstChildElement(name);

        if (prop.hasAttribute("type") && prop.hasAttribute("value"))
        {
            QVariant value;
            value.setValue(prop.attribute("value"));

            if (property.isEnumType())
            {
                value.convert(QVariant::Int);
                property.write(this, value);
            }
            else if (value.convert((QVariant::Type)prop.attribute("type").toInt()))
            {
                property.write(this, value);
            }
        }
        else if (prop.hasAttribute("x") && prop.hasAttribute("y"))
        {
            QVariant value;
            QPointF point;
            point.setX(prop.attribute("x", "0").toDouble());
            point.setY(prop.attribute("y", "0").toDouble());
            value.setValue(point);
            property.write(this, value);
        }
        else if (prop.hasAttribute("type") && prop.hasAttribute("count"))
        {
            QVariant value;
            QStringList stringList;

            QDomElement child = prop.firstChildElement("listitem");
            while (!child.isNull())
            {
                stringList << child.attribute("value");
                child = child.nextSiblingElement("listitem");
            }

            value.setValue(stringList);
            property.write(this, value);
        }
    }

//    foreach (Property* prop, m_properties)
//    {
//        QDomElement propElem = componentElement.firstChildElement(prop->getName());

//        if (!propElem.isNull())
//        {
//            prop->deserialize(propElem);
//        }
//    }

    privateDeserialize(componentElement);

    return true;
}

QStringList Component::getCauseList()
{
    QMetaObject metaObject = *this->metaObject();
    QMetaMethod method;
    QStringList causeList;

    for (int i = 0; i<metaObject.methodCount(); i++)
    {
        method = metaObject.method(i);
        QString signature = metaObject.normalizedSignature(method.signature());
        if (method.methodType() == QMetaMethod::Signal && signature.startsWith("cause"))
        {
            causeList << signature;
        }
    }

    return causeList;
}

QStringList Component::getEffectList()
{
    QMetaObject metaObject = *this->metaObject();
    QMetaMethod method;
    QStringList effectList;

    for (int i = 0; i<metaObject.methodCount(); i++)
    {
        method = metaObject.method(i);
        QString signature = metaObject.normalizedSignature(method.signature());
        if (method.methodType() == QMetaMethod::Slot && signature.startsWith("effect"))
        {
            effectList << signature;
        }
    }

    return effectList;
}
