#include "inputcomponent.h"

InputComponent::InputComponent(GameObject *parentObject) :
    Component(parentObject)
{
    this->setObjectName("Input Component");
    qRegisterMetaType<Qt::Key>("Key");

    m_key = Qt::Key_0;
}

QSet<QString> InputComponent::getEditProperties()
{
    QSet<QString> properties;
    properties << "key" << "tag";
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
        emit causeKeyPressed();
    }
}

void InputComponent::keyReleaseEvent(QKeyEvent *ke)
{
    Qt::Key pressed = (Qt::Key)ke->key();

    if (m_pressed.contains(pressed) && pressed == m_key)
    {
        emit causeKeyReleased();
    }

    m_pressed.remove(pressed);
}
