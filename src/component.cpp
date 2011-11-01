#include "component.h"
#include "gameobject.h"

Component::Component(GameObject *parentObject) :
    //QObject(parentObject),
    m_parentObject(parentObject)
{
}

QDomElement Component::serialize(QDomDocument *document)
{
    this->prepareForSerialization();

    QDomElement componentElement = document->createElement("component");

    componentElement.setAttribute("name", this->objectName());

    for (int i = 0; i<this->metaObject()->propertyCount(); i++)
    {
        QMetaProperty property = this->metaObject()->property(i);
        QString name = property.name();

        if (property.isValid() && property.type() != QVariant::Invalid && property.userType())
        {
            QVariant value = property.read(this);

            if (property.isEnumType())
            {
                QDomElement prop = document->createElement(name);
                prop.setAttribute("type", value.type());
                QVariant intValue = *reinterpret_cast<const int *>(value.constData());
                prop.setAttribute("value", intValue.toString());
                componentElement.appendChild(prop);
            }
            else if (value.type() == QVariant::StringList)
            {
                QDomElement prop = document->createElement(name);

                QStringList stringList = value.toStringList();
                foreach(QString s, stringList)
                {
                    QDomElement child = document->createElement("listitem");
                    child.setAttribute("value", s);
                    prop.appendChild(child);
                }

                prop.setAttribute("type", value.type());
                prop.setAttribute("count", stringList.count());
                componentElement.appendChild(prop);
            }
            else
            {
                QDomElement prop = document->createElement(name);
                prop.setAttribute("type", value.type());
                prop.setAttribute("value", value.toString());
                componentElement.appendChild(prop);
            }
        }
    }
    return componentElement;
}

bool Component::deserialize(const QDomElement &objectElement)
{
    for (int i = 0; i<this->metaObject()->propertyCount(); i++)
    {
        QMetaProperty property = this->metaObject()->property(i);
        QString name = property.name();
        QDomElement prop = objectElement.firstChildElement(name);

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

    return true;
}
