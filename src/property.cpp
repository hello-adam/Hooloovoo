#include "property.h"
#include <QtGui>
#include "gameobject.h"
#include "editwidgets/pointedit.h"
#include "editwidgets/stringedit.h"
#include "editwidgets/doubleedit.h"
#include "editwidgets/coloredit.h"
#include "editwidgets/integeredit.h"
#include "editwidgets/booleanedit.h"
#include "editwidgets/stringlistedit.h"
#include "editwidgets/enumedit.h"

Property::Property(QObject *propertyHolder, QString propertyName, ValueHelperDialog helper) :
    m_propertyHolder(propertyHolder),
    m_propertyName(propertyName),
    m_helper(helper)
{
}

QVariant Property::getValue()
{
    if (!m_propertyHolder)
        return QVariant();

    return m_propertyHolder->property(qPrintable(m_propertyName));
}

void Property::setValue(QVariant value)
{
    if (!m_propertyHolder)
        return;

    m_propertyHolder->setProperty(qPrintable(m_propertyName), value);

    Component* component = qobject_cast<Component*>(m_propertyHolder);
    if (component)
    {
        component->instantiate();

        GameObject* gameObject = qobject_cast<GameObject*>(component);

        if (gameObject)
        {
            gameObject->initiateObject();
        }
    }
}

QDomElement Property::serialize()
{
    QDomDocument doc;
    QDomElement prop = doc.createElement(m_propertyName);

    QMetaProperty property = m_propertyHolder->metaObject()->property(m_propertyHolder->metaObject()->indexOfProperty(qPrintable(m_propertyName)));

    if (property.isValid() && property.type() != QVariant::Invalid && property.userType())
    {
        QVariant value = m_propertyHolder->property(qPrintable(m_propertyName));

        if (property.isEnumType())
        {
            prop.setAttribute("type", value.type());
            QVariant intValue = *reinterpret_cast<const int *>(value.constData());
            prop.setAttribute("value", intValue.toString());
        }
        else if (value.type() == QVariant::StringList)
        {
            QStringList stringList = value.toStringList();
            foreach(QString s, stringList)
            {
                QDomElement child = doc.createElement("listitem");
                child.setAttribute("value", s);
                prop.appendChild(child);
            }

            prop.setAttribute("type", value.type());
            prop.setAttribute("count", stringList.count());
        }
        else if (value.type() == QVariant::PointF)
        {
            prop.setAttribute("type", value.type());
            prop.setAttribute("x", value.toPointF().x());
            prop.setAttribute("y", value.toPointF().y());
        }
        else
        {
            prop.setAttribute("type", value.type());
            prop.setAttribute("value", value.toString());
        }
    }

    return prop;
}

void Property::deserialize(const QDomElement &prop)
{
    if (!m_propertyHolder)
        return;

    QMetaProperty property = m_propertyHolder->metaObject()->property(m_propertyHolder->metaObject()->indexOfProperty(qPrintable(m_propertyName)));

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

    m_value = m_propertyHolder->property(qPrintable(m_propertyName));
}

QString Property::getDisplayName()
{
    return m_propertyName;
}

QString Property::getDisplayValue()
{
    if (!m_propertyHolder)
        return QString("...");

    QMetaProperty property = m_propertyHolder->metaObject()->property(m_propertyHolder->metaObject()->indexOfProperty(qPrintable(m_propertyName)));
    QVariant value = m_propertyHolder->property(qPrintable(m_propertyName));

    if (property.isEnumType())
    {
        QMetaEnum enumerator = property.enumerator();
        return enumerator.valueToKey(*reinterpret_cast<const int *>(value.constData()));
    }
    else if (value.type() == QVariant::String)
    {
        return value.toString();
    }
    else if (value.type() == QVariant::Double)
    {
        QString str;
        str.setNum(value.toDouble(), 'g', 2);
        return str;
    }
    else if (value.type() == QVariant::Int)
    {
        QString str;
        str.setNum(value.toInt());
        return str;
    }
    else if (value.type() == QVariant::Bool)
    {
        if (value.toBool())
        {
            return "True";
        }
        else
        {
            return "False";
        }
    }
    else if (value.type() == QVariant::StringList)
    {
        QString str = value.toStringList().join(", ");
        str = "[ " + str + "]";
        return str;
    }
    else if (value.type() == QVariant::Color)
    {
        QColor color = value.value<QColor>();

        QString red;
        red.setNum(color.red());
        QString green;
        green.setNum(color.green());
        QString blue;
        blue.setNum(color.blue());

        return "Red:" + red + ", Green:" + green + ", Blue:" + blue;
    }
    else if (value.type() == QVariant::PointF)
    {
        QPointF point = value.toPointF();

        QString x;
        x.setNum(point.x());
        QString y;
        y.setNum(point.y());

        return "[ " + x + ", " + y + "]";
    }

    return QString("...");
}

QWidget* Property::createEditorWidget()
{
    if (!m_propertyHolder)
        return new QWidget();

    QMetaProperty property = m_propertyHolder->metaObject()->property(m_propertyHolder->metaObject()->indexOfProperty(qPrintable(m_propertyName)));
    QVariant value = m_propertyHolder->property(qPrintable(m_propertyName));

    if (property.isEnumType())
    {
        return new EnumEdit(this);
    }
    else if (value.type() == QVariant::String)
    {
        return new StringEdit(this);
    }
    else if (value.type() == QVariant::Double)
    {
        return new DoubleEdit(this);
    }
    else if (value.type() == QVariant::Int)
    {
        return new IntegerEdit(this);
    }
    else if (value.type() == QVariant::Bool)
    {
        return new BooleanEdit(this);
    }
    else if (value.type() == QVariant::StringList)
    {
        return new StringListEdit(this);
    }
    else if (value.type() == QVariant::Color)
    {
        return new ColorEdit(this);
    }
    else if (value.type() == QVariant::PointF)
    {
        return new PointEdit(this);
    }

    return 0;
}
