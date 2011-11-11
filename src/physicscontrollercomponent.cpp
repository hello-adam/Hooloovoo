#include "physicscontrollercomponent.h"
#include "gamecore.h"

PhysicsControllerComponent::PhysicsControllerComponent(GameObject *parentObject) :
    Component(parentObject)
{
    this->setObjectName("Physics Controller Component");

    m_physicsComponent = 0;
    m_value = QPointF(0, 0);
    m_engageTrigger = "";
    m_releaseTrigger = "";
    m_controlType = PhysicsControllerComponent::ConstantVelocity;

    qRegisterMetaType<PhysicsControllerComponent::PhysicsControlType>("PhysicsControlType");

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

    connect(parentObject, SIGNAL(sendLocalEvent(QString)),
            this, SLOT(reactToLocalEvent(QString)));
}

PhysicsControllerComponent::~PhysicsControllerComponent()
{
}

QSet<QString> PhysicsControllerComponent::getEditProperties()
{
    QSet<QString> properties;
    properties << "engageTrigger" << "releaseTrigger" << "controlType" << "value";
    return properties;
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

void PhysicsControllerComponent::reactToLocalEvent(QString trigger)
{
    if (!m_physicsComponent)
        return;
    if (!m_physicsComponent->getBody())
        return;

    if (trigger == m_engageTrigger)
    {
        if (m_controlType == PhysicsControllerComponent::Impulse)
        {
            m_physicsComponent->getBody()->ApplyLinearImpulse(b2Vec2(m_value.x(), m_value.y()), m_physicsComponent->getBody()->GetPosition());
        }
        else
        {
            connect(GameCore::getInstance(), SIGNAL(timerTick()),
                    this, SLOT(reactToTimerTick()));
        }
    }
    else if (trigger == m_releaseTrigger)
    {
        if (m_controlType == PhysicsControllerComponent::Impulse)
        {
            return;
        }
        b2Body* body = m_physicsComponent->getBody();
        b2Vec2 nullifyValue = b2Vec2(m_value.x()*-1, m_value.y()*-1);

        if (m_controlType == PhysicsControllerComponent::ConstantVelocity)
        {
            body->SetLinearVelocity(nullifyValue + body->GetLinearVelocity());
        }

        disconnect(GameCore::getInstance(), SIGNAL(timerTick()),
                   this, SLOT(reactToTimerTick()));
    }
}

void PhysicsControllerComponent::reactToTimerTick()
{
    if (!m_physicsComponent)
        return;
    if (!m_physicsComponent->getBody())
        return;

    b2Body* body = m_physicsComponent->getBody();
    QPointF netValue = m_value;

    if (m_controlType == PhysicsControllerComponent::ConstantVelocity)
    {
        if (netValue.x() == 0)
        {
            netValue.setX(body->GetLinearVelocity().x);
        }
        if (netValue.y() == 0)
        {
            netValue.setY(body->GetLinearVelocity().y);
        }
        body->SetLinearVelocity(b2Vec2(netValue.x(), netValue.y()));
    }
    else if (m_controlType == PhysicsControllerComponent::ConstantAcceleration)
    {
        body->ApplyForceToCenter(b2Vec2(netValue.x(), netValue.y()));
    }
}
