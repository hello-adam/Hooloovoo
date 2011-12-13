#include "contactcomponent.h"
#include "gameobject.h"
#include "physicsmanager.h"

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

    connect(&PhysicsManager::getInstance(), SIGNAL(stepTaken()),
            this, SLOT(contactCheck()));

    if (m_physicsComponent)
    {
        connect(m_physicsComponent, SIGNAL(enteringContact(int)), this, SLOT(enterContact(int)));
        connect(m_physicsComponent, SIGNAL(leavingContact(int)), this, SLOT(leaveContact(int)));
    }
}

QSet<QString> ContactComponent::getEditProperties()
{
    QSet<QString> properties;
    properties << "tag";
    return properties;
}

void ContactComponent::enterContact(int contactObjectID)
{
    m_newContacts.insert(contactObjectID);
}

void ContactComponent::leaveContact(int contactObjectID)
{
    m_removedContacts.insert(contactObjectID);
}

void ContactComponent::contactCheck()
{
    if (m_newContacts.count() > 0)
    {
        emit causeEnterContact();
    }
    if (m_removedContacts.count() > 0)
    {
        emit causeLeaveContact();
    }

    bool hadContact = !m_contacts.isEmpty();

    foreach (int id, m_newContacts)
    {
        m_contacts.insert(id);
    }
    foreach (int id, m_removedContacts)
    {
        m_contacts.remove(id);
    }

    if (hadContact && m_contacts.isEmpty())
    {
        emit causeNoContact();
    }

    m_newContacts.clear();
    m_removedContacts.clear();
}

void ContactComponent::checkForAddedPhysicsComponent(Component* c)
{
    if (m_physicsComponent)
        return;

    if (qobject_cast<PhysicsComponent*>(c))
        m_physicsComponent = qobject_cast<PhysicsComponent*>(c);

    if (m_physicsComponent)
    {
        connect(m_physicsComponent, SIGNAL(enteringContact(int)), this, SLOT(enterContact(int)));
        connect(m_physicsComponent, SIGNAL(leavingContact(int)), this, SLOT(leaveContact(int)));
    }
}

void ContactComponent::checkForRemovedPhysicsComponent(Component* c)
{
    if (!m_physicsComponent)
        return;

    if (qobject_cast<PhysicsComponent*>(c))
        m_physicsComponent = 0;
}
