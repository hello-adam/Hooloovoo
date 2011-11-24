#include "contactcomponent.h"
#include "gameobject.h"

ContactComponent::ContactComponent(GameObject *parentObject) :
    Component(parentObject)
{
    this->setObjectName("Contact Component");

    m_physicsComponent = 0;

    QList<Component*> components = parentObject->getComponents();
    foreach (Component* c, components)
    {
        if (qobject_cast<PhysicsComponent*>(c))
        {
            m_physicsComponent = qobject_cast<PhysicsComponent*>(c);
            break;
        }
    }

    connect(parentObject, SIGNAL(componentAdded(Component*)),
            this, SLOT(checkForAddedPhysicsComponent(Component*)));
    connect(parentObject, SIGNAL(componentRemoved(Component*)),
            this, SLOT(checkForRemovedPhysicsComponent(Component*)));

    if (m_physicsComponent)
        connect(m_physicsComponent, SIGNAL(enteringContact(GameObject*)), this, SLOT(reactToContact(GameObject*)));
}

QSet<QString> ContactComponent::getEditProperties()
{
    QSet<QString> properties;
    properties << "thisObjectTrigger" << "contactedObjectTrigger";
    return properties;
}

void ContactComponent::reactToContact(GameObject *contactObject)
{
    if (!m_localTrigger.isEmpty())
        emit sendLocalTrigger(m_localTrigger);

    if (m_contactTrigger.isEmpty())
        return;

    contactObject->emitLocalTrigger(m_contactTrigger);
}

void ContactComponent::checkForAddedPhysicsComponent(Component* c)
{
    if (m_physicsComponent)
        return;

    if (qobject_cast<PhysicsComponent*>(c))
        m_physicsComponent = qobject_cast<PhysicsComponent*>(c);

    if (m_physicsComponent)
        connect(m_physicsComponent, SIGNAL(enteringContact(GameObject*)), this, SLOT(reactToContact(GameObject*)));
}

void ContactComponent::checkForRemovedPhysicsComponent(Component* c)
{
    if (!m_physicsComponent)
        return;

    if (qobject_cast<PhysicsComponent*>(c))
        m_physicsComponent = 0;
}
