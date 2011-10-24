#include "physicscontrollercomponent.h"

PhysicsControllerComponent::PhysicsControllerComponent(GameObject *parentObject) :
    Component(parentObject)
{
    this->setObjectName("Physics Controller Component");

    m_physicsComponent = 0;

    QList<Component*> components = parentObject->getComponents();
    foreach (Component* c, components)
    {
        if (c->objectName() == "Physics Component")
        {
            m_physicsComponent = qobject_cast<PhysicsComponent*>(c);
            break;
        }
    }
}

PhysicsControllerComponent::~PhysicsControllerComponent()
{
}

QSet<QString> PhysicsControllerComponent::getEditProperties()
{
    QSet<QString> properties;
    //properties << "isDynamic" << "xVelocity" << "yVelocity" << "angularVelocity" << "density" << "friction";
    return properties;
}

void PhysicsControllerComponent::keyPressEvent(QKeyEvent *ke)
{
    qDebug() << "Got Key Press!";
    if (!m_physicsComponent)
        return;

    bool hasContact = false;
    b2ContactEdge* edge = m_physicsComponent->getBody()->GetContactList();
    if (edge)
    {
        hasContact = true;
    }


    if (ke->key() == Qt::Key_W)
    {
        if (hasContact)
        {
            m_physicsComponent->getBody()->ApplyLinearImpulse(b2Vec2(0, 3000), m_physicsComponent->getBody()->GetPosition());
        }
    }
    else if (ke->key() == Qt::Key_A)
    {
        if (hasContact)
        {
            m_physicsComponent->getBody()->ApplyLinearImpulse(b2Vec2(-300, 0), m_physicsComponent->getBody()->GetPosition());
        }
    }
    else if (ke->key() == Qt::Key_D)
    {
        if (hasContact)
        {
            m_physicsComponent->getBody()->ApplyLinearImpulse(b2Vec2(300, 0), m_physicsComponent->getBody()->GetPosition());
        }
    }
}
