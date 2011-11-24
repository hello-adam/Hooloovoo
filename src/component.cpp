#include "component.h"
#include "gameobject.h"

Component::Component(GameObject *parentObject) :
    //QObject(parentObject),
    m_parentObject(parentObject)
{
    m_tag = "";

    connect(this, SIGNAL(sendLocalTrigger(QString)),
            parentObject, SIGNAL(sendLocalTrigger(QString)));
    connect(this, SIGNAL(sendGlobalTrigger(QString)),
            parentObject, SIGNAL(sendGlobalTrigger(QString)));
    connect(parentObject, SIGNAL(sendLocalTrigger(QString)),
            this, SLOT(checkTrigger(QString)));
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
            else if (value.type() == QVariant::PointF)
            {
                QDomElement prop = document->createElement(name);
                prop.setAttribute("type", value.type());
                prop.setAttribute("x", value.toPointF().x());
                prop.setAttribute("y", value.toPointF().y());
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

    return true;
}

void Component::checkTrigger(QString trigger)
{
    if (trigger.contains(QRegExp("Tag(.*)")))
    {
        int start = trigger.indexOf("Tag(")+4;
        int end = trigger.indexOf(")", start);
        QString tag = trigger.mid(start, end-start);

        trigger.remove("Tag(" + tag + ")");

        if (m_tag != tag)
            return;
    }

    if (trigger.contains(QRegExp("Property(.*)")))
    {
        int start = trigger.indexOf("Property(")+9;
        int end = trigger.indexOf(")", start);
        QString argString = trigger.mid(start, end-start);
        QStringList args = argString.split(',', QString::KeepEmptyParts);

        trigger.remove("Property(" + argString + ")");

        reactToPropertyTrigger(args);
    }

    if (!trigger.isEmpty())
        reactToTrigger(trigger);
}

void Component::reactToPropertyTrigger(QStringList args)
{
    qDebug() << args;

    if (args.count() == 3)
    {
        if (args.at(0) == this->objectName())
        {
            if (this->metaObject()->indexOfProperty(args.at(1).toStdString().c_str()) >= 0)
            {
                this->setProperty(args.at(1).toStdString().c_str(), QVariant(args.at(2)));
            }
        }
    }
}
