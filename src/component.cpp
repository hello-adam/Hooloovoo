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
    QDomDocument doc;

    this->prepareForSerialization();

    QDomElement componentElement = doc.createElement("component");

    componentElement.setAttribute("name", this->objectName());
    componentElement.setAttribute("id", m_ID);

    for (int i = 0; i<this->metaObject()->propertyCount(); i++)
    {
        QMetaProperty property = this->metaObject()->property(i);
        QString name = property.name();

        if (property.isValid() && property.type() != QVariant::Invalid && property.userType())
        {
            QVariant value = property.read(this);

            if (property.isEnumType())
            {
                QDomElement prop = doc.createElement(name);
                prop.setAttribute("type", value.type());
                QVariant intValue = *reinterpret_cast<const int *>(value.constData());
                prop.setAttribute("value", intValue.toString());
                componentElement.appendChild(prop);
            }
            else if (value.type() == QVariant::StringList)
            {
                QDomElement prop = doc.createElement(name);

                QStringList stringList = value.toStringList();
                foreach(QString s, stringList)
                {
                    QDomElement child = doc.createElement("listitem");
                    child.setAttribute("value", s);
                    prop.appendChild(child);
                }

                prop.setAttribute("type", value.type());
                prop.setAttribute("count", stringList.count());
                componentElement.appendChild(prop);
            }
            else if (value.type() == QVariant::PointF)
            {
                QDomElement prop = doc.createElement(name);
                prop.setAttribute("type", value.type());
                prop.setAttribute("x", value.toPointF().x());
                prop.setAttribute("y", value.toPointF().y());
                componentElement.appendChild(prop);
            }
            else
            {
                QDomElement prop = doc.createElement(name);
                prop.setAttribute("type", value.type());
                prop.setAttribute("value", value.toString());
                componentElement.appendChild(prop);
            }
        }
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
