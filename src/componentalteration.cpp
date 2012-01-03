#include "componentalteration.h"
#include "editwidgets/alterationedit.h"
#include "component.h"

ComponentAlteration::ComponentAlteration(QObject *propertyHolder, QString propertyName, ValueHelperDialog helper) :
    Property(propertyHolder, propertyName, helper)
{
    m_propertyValueProperty = new Property(this, "propertyValue");
}

bool ComponentAlteration::alterComponent(Component* component)
{
    return true;
}

QString ComponentAlteration::dataToString()
{
    QDomDocument doc;
    QDomElement data = doc.createElement("componentalteration");

    data.setAttribute("name", m_componentName);
    data.setAttribute("tag", m_componentTag);
    data.setAttribute("property", m_componentProperty);

    data.appendChild(m_propertyValueProperty->serialize());

    QString text;
    QTextStream stream(&text);
    data.save(stream, 2);

    return text;
}

void ComponentAlteration::dataFromString(QString data)
{
    QDomDocument doc;
    doc.setContent(data);
    QDomElement specs = doc.documentElement();

    m_componentName = specs.attribute("name");
    m_componentTag = specs.attribute("tag");
    m_componentProperty = specs.attribute("property");
    m_propertyValueProperty->deserialize(specs.firstChildElement("propertyValue"));

    qDebug() << "value that was read: " << m_propertyValueProperty->getValue();
}

QVariant ComponentAlteration::getValue()
{
    QString value = Property::getValue().toString();
    qDebug() << "get value value:" << value;
    return QVariant(value);
}

void ComponentAlteration::setValue(QVariant value)
{
    qDebug()<< "setting value";
    dataFromString(value.toString());

    Property::setValue(value);
}

QWidget* ComponentAlteration::createEditorWidget()
{
    return new AlterationEdit(this);
}
