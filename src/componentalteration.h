#ifndef COMPONENTALTERATION_H
#define COMPONENTALTERATION_H

#include "property.h"
class EditWidget;
class Component;

class ComponentAlteration : public Property
{
    Q_OBJECT
    Q_PROPERTY(QVariant propertyValue READ getPropertyValue WRITE setPropertyValue)

public:
    explicit ComponentAlteration(QObject *propertyHolder, QString propertyName, ValueHelperDialog helper = NoHelperDialog);

    enum AlterationType
    {
        SetValue, Add
    };

    bool alterComponent(Component* component);

    QString dataToString();
    void dataFromString(QString data);

    void setValue(QVariant value);
    QVariant getValue();

    QWidget* createEditorWidget();

    void setPropertyValue(QVariant value) {m_propertyValue = value;}
    QVariant getPropertyValue() {return m_propertyValue;}

    void setName(QString name) {m_componentName = name;}
    QString getName() {return m_componentName;}
    void setTag(QString tag) {m_componentTag = tag;}
    QString getTag() {return m_componentTag;}
    void setPropertyName(QString propName) {m_componentProperty = propName;}
    QString getPropertyName() {return m_componentProperty;}

    Property* getValueProperty() {return m_propertyValueProperty;}

private:
    QString m_componentName;
    QString m_componentTag;
    QString m_componentProperty;
    QVariant m_propertyValue;
    Property *m_propertyValueProperty;

    AlterationType m_alterType;

signals:

public slots:

};

#endif // COMPONENTALTERATION_H
