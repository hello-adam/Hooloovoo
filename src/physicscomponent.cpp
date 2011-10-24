#include "physicscomponent.h"
#include "gameobject.h"
#include "physicsmanager.h"
#include <QDebug>

PhysicsComponent::PhysicsComponent(GameObject *parentObject) :
    Component(parentObject)
{
    m_body = 0;

    this->setObjectName("Physics Component");
    m_isDynamic = false;
    m_density = 20.0f;
    m_friction = 0.03f;
    m_vx = 0;
    m_vy = 0;
    m_vangle = 0;

    connect (PhysicsManager::getInstance(), SIGNAL(stepTaken()),
             this, SLOT(updateParent()));

    connect (m_parentObject, SIGNAL(sendX(double)),
             this, SLOT(setX(double)));
    connect (m_parentObject, SIGNAL(sendY(double)),
             this, SLOT(setY(double)));
    connect (m_parentObject, SIGNAL(sendRotation(double)),
             this, SLOT(setAngle(double)));

    instantiate();
}

PhysicsComponent::~PhysicsComponent()
{
    if (m_body)
    {
        PhysicsManager::getInstance()->getWorld()->DestroyBody(m_body);
    }
}

QSet<QString> PhysicsComponent::getEditProperties()
{
    QSet<QString> properties;
    properties << "isDynamic" << "xVelocity" << "yVelocity" << "angularVelocity" << "density" << "friction";
    return properties;
}

void PhysicsComponent::prepareForSerialization()
{
    if (m_body)
    {
        m_vx = m_body->GetLinearVelocity().x;
        m_vy = m_body->GetLinearVelocity().y;
        m_vangle = m_body->GetAngularVelocity();
    }
}

void PhysicsComponent::instantiate()
{
    if (m_body)
    {
        PhysicsManager::getInstance()->getWorld()->DestroyBody(m_body);
    }

    b2BodyDef bodyDef;
    if (m_isDynamic)
    {
        bodyDef.type = b2_dynamicBody;
    }
    bodyDef.position.Set(m_parentObject->pos().x()/20.0f, m_parentObject->pos().y()/-20.0f);
    bodyDef.angle = -(m_parentObject->rotation() * (2 * 3.14159)) / 360.0;

    m_body = PhysicsManager::getInstance()->getWorld()->CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(m_parentObject->width()/40.0f, m_parentObject->height()/40.0f);

    b2FixtureDef fixture;
    fixture.shape = &shape;
    fixture.friction = m_friction;
    fixture.density = m_density;

    m_body->CreateFixture(&fixture);

    //m_body->SetAngularVelocity(m_vangle);
    m_body->SetLinearVelocity(b2Vec2(m_vx, m_vy));
}

void PhysicsComponent::updateParent()
{
    if (m_body)
    {
        b2Vec2 position = m_body->GetPosition();
        float32 angle = m_body->GetAngle();
        m_parentObject->setPos(position.x*20.0f, position.y*-20.0f);
        m_parentObject->setRotation(-(angle * 360.0) / (2 * 3.14159));
    }
}

void PhysicsComponent::setX(double x)
{
    if (m_body)
    {
        m_body->SetTransform(b2Vec2(x/20.0f, m_body->GetPosition().y), m_body->GetAngle());
    }
}

void PhysicsComponent::setY(double y)
{
    if (m_body)
    {
        m_body->SetTransform(b2Vec2(m_body->GetPosition().x, y/-20.0f), m_body->GetAngle());
    }
}

void PhysicsComponent::setAngle(double degrees)
{
    if (m_body)
    {
        m_body->SetTransform(b2Vec2(m_body->GetPosition().x, m_body->GetPosition().y), -(degrees * (2 * 3.14159)) / 360.0);
    }
}
