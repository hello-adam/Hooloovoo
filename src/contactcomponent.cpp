#include "contactcomponent.h"
#include "gameobject.h"
#include "physicsmanager.h"

ContactComponent::ContactComponent(GameObject *parentObject) :
    Component(parentObject)
{
    this->setObjectName("Contact Component");

    m_physicsComponent = 0;
    m_madeContact = false;

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

    connect(&PhysicsManager::getInstance(), SIGNAL(stepTaken()),
            this, SLOT(contactCheck()));

    if (m_physicsComponent)
        connect(m_physicsComponent, SIGNAL(enteringContact(GameObject*)), this, SLOT(reactToContact(GameObject*)));
}

QSet<QString> ContactComponent::getEditProperties()
{
    QSet<QString> properties;
    properties << "tag";
    return properties;
}

void ContactComponent::enterContact(GameObject *contactObject)
{
    m_newContacts.insert(contactObject);
}

void ContactComponent::leaveContact(GameObject *contactObject)
{
    m_removedContacts.remove(contactObject);
}

void ContactComponent::contactCheck()
{
    if (m_newContacts)
}

void ContactComponent::checkForAddedPhysicsComponent(Component* c)
{
    if (m_physicsComponent)
        return;

    if (qobject_cast<PhysicsComponent*>(c))
        m_physicsComponent = qobject_cast<PhysicsComponent*>(c);

    if (m_physicsComponent)
    {
        connect(m_physicsComponent, SIGNAL(enteringContact(GameObject*)), this, SLOT(enterContact(GameObject*)));
        connect(m_physicsComponent, SIGNAL(leavingContact(GameObject*)), this, SLOT(leaveContact(GameObject*)));
    }
}

void ContactComponent::checkForRemovedPhysicsComponent(Component* c)
{
    if (!m_physicsComponent)
        return;

    if (qobject_cast<PhysicsComponent*>(c))
        m_physicsComponent = 0;
}
