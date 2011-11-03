#include "inputcomponent.h"

InputComponent::InputComponent(GameObject *parentObject) :
    Component(parentObject)
{
    this->setObjectName("Input Component");
    qRegisterMetaType<Qt::Key>("Key");
    qRegisterMetaType<InputComponent::TriggerType>("TriggerType");

    m_key = Qt::Key_0;
    m_trigger = "";
    m_type = InputComponent::Local;
}

QSet<QString> InputComponent::getEditProperties()
{
    QSet<QString> properties;
    properties << "key" << "trigger" << "triggerType";
    return properties;
}

void InputComponent::keyPressEvent(QKeyEvent *ke)
{
    Qt::Key pressed = (Qt::Key)ke->key();

    if (m_pressed.contains(pressed))
        return;

    m_pressed.insert(pressed);

    if (pressed == m_key)
    {
        if (m_type == InputComponent::Local)
        {
            emit sendLocalEvent(m_trigger);
        }
        else if (m_type == InputComponent::Global)
        {
            emit sendGlobalEvent(m_trigger);
        }
    }
}

void InputComponent::keyReleaseEvent(QKeyEvent *ke)
{
    Qt::Key pressed = (Qt::Key)ke->key();

    m_pressed.remove(pressed);
}
