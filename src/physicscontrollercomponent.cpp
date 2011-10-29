#include "physicscontrollercomponent.h"

PhysicsControllerComponent::PhysicsControllerComponent(GameObject *parentObject) :
    Component(parentObject)
{
    this->setObjectName("Physics Controller Component");

    m_physicsComponent = 0;
    m_jumpImpulse = 200;
    m_movementSpeed = 20;

    QList<Component*> components = parentObject->getComponents();
    foreach (Component* c, components)
    {
        if (c->objectName() == "Physics Component")
        {
            m_physicsComponent = qobject_cast<PhysicsComponent*>(c);
            break;
        }
    }

    connect(parentObject, SIGNAL(componentAdded(Component*)),
            this, SLOT(checkForAddedPhysicsComponent(Component*)));
    connect(parentObject, SIGNAL(componentRemoved(Component*)),
            this, SLOT(checkForRemovecPhysicsComponent(Component*)));
}

PhysicsControllerComponent::~PhysicsControllerComponent()
{
}

QSet<QString> PhysicsControllerComponent::getEditProperties()
{
    QSet<QString> properties;
    properties << "jumpImpulse" << "movementSpeed";
    return properties;
}

void PhysicsControllerComponent::keyPressEvent(QKeyEvent *ke)
{
    if (!m_physicsComponent)
        return;

    if (m_pressedKeys.contains(ke->key()))
        return;

    m_pressedKeys.insert(ke->key());

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
            m_physicsComponent->getBody()->ApplyLinearImpulse(b2Vec2(0, m_jumpImpulse), m_physicsComponent->getBody()->GetPosition());
        }
    }
    else if (ke->key() == Qt::Key_A)
    {
        m_physicsComponent->getBody()->SetLinearVelocity(b2Vec2(-1*m_movementSpeed, m_physicsComponent->getBody()->GetLinearVelocity().y));
    }
    else if (ke->key() == Qt::Key_D)
    {
        m_physicsComponent->getBody()->SetLinearVelocity(b2Vec2(m_movementSpeed, m_physicsComponent->getBody()->GetLinearVelocity().y));
    }
}
void PhysicsControllerComponent::keyReleaseEvent(QKeyEvent *ke)
{
    if (!m_physicsComponent)
        return;

    if (!m_pressedKeys.contains(ke->key()))
        return;

    m_pressedKeys.remove(ke->key());

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
        }
    }
    else if (ke->key() == Qt::Key_A)
    {
        m_physicsComponent->getBody()->SetLinearVelocity(b2Vec2(0, m_physicsComponent->getBody()->GetLinearVelocity().y));
    }
    else if (ke->key() == Qt::Key_D)
    {
        m_physicsComponent->getBody()->SetLinearVelocity(b2Vec2(0, m_physicsComponent->getBody()->GetLinearVelocity().y));
    }
}


void PhysicsControllerComponent::checkForAddedPhysicsComponent(Component* c)
{
    if (m_physicsComponent)
        return;

    if (qobject_cast<PhysicsComponent*>(c))
        m_physicsComponent = qobject_cast<PhysicsComponent*>(c);
}

void PhysicsControllerComponent::checkForRemovecPhysicsComponent(Component* c)
{
    if (!m_physicsComponent)
        return;

    if (qobject_cast<PhysicsComponent*>(c))
        m_physicsComponent = 0;
}
