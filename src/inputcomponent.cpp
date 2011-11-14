#include "inputcomponent.h"

InputComponent::InputComponent(GameObject *parentObject) :
    Component(parentObject)
{
    this->setObjectName("Input Component");
    qRegisterMetaType<Qt::Key>("Key");
    qRegisterMetaType<InputComponent::TriggerType>("TriggerType");

    m_key = Qt::Key_0;
    m_trigger = "";
    m_releaseTrigger = "";
    m_type = InputComponent::Local;
}

QSet<QString> InputComponent::getEditProperties()
{
    QSet<QString> properties;
    properties << "key" << "trigger" << "triggerType" << "releaseTrigger" << "tag";
    return properties;
}

void InputComponent::handleKeyEvent(QKeyEvent *ke)
{
    if (ke->type() == QKeyEvent::KeyPress)
        keyPressEvent(ke);

    else if (ke->type() == QKeyEvent::KeyRelease)
        keyReleaseEvent(ke);
}

void InputComponent::keyPressEvent(QKeyEvent *ke)
{
    Qt::Key pressed = (Qt::Key)ke->key();

    if (m_pressed.contains(pressed))
    {
        return;
    }

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

    if (!m_releaseTrigger.isEmpty() && m_pressed.contains(pressed) && pressed == m_key)
    {
        if (m_type == InputComponent::Local)
        {
            emit sendLocalEvent(m_releaseTrigger);
        }
        else if (m_type == InputComponent::Global)
        {
            emit sendGlobalEvent(m_releaseTrigger);
        }
    }

    m_pressed.remove(pressed);
}
