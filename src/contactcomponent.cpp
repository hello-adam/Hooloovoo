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
            this, SLOT(checkForRemovecPhysicsComponent(Component*)));

    if (m_physicsComponent)
        connect(m_physicsComponent, SIGNAL(enteringContact(GameObject*)), this, SLOT(reactToContact(GameObject*)));
}

QSet<QString> ContactComponent::getEditProperties()
{
    QSet<QString> properties;
    properties << "localTrigger" << "contactCondition" << "contactTrigger";
    return properties;
}

void ContactComponent::reactToContact(GameObject *contactObject)
{
    if (!m_localTrigger.isEmpty())
        emit sendLocalEvent(m_localTrigger);

    if (m_contactTrigger.isEmpty())
        return;

    if (m_contactCondition.isEmpty() || contactObject->getTag().contains(m_contactCondition))
        contactObject->emitLocalEvent(m_contactTrigger);
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

void ContactComponent::checkForRemovecPhysicsComponent(Component* c)
{
    if (!m_physicsComponent)
        return;

    if (qobject_cast<PhysicsComponent*>(c))
        m_physicsComponent = 0;
}
