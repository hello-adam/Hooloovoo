#include "physicscomponent.h"
#include "gameobject.h"
#include "physicsmanager.h"
#include <QDebug>

PhysicsComponent::PhysicsComponent(GameObject *parentObject) :
    Component(parentObject)
{
    m_body = 0;

    this->setObjectName("Physics Component");
    qRegisterMetaType<PhysicsComponent::BodyType>("BodyType");

    m_type = PhysicsComponent::Static;
    m_staticRotation = false;
    m_isSensor = false;
    m_density = 1.0f;
    m_friction = 0.03f;
    m_vx = 0;
    m_vy = 0;
    m_vangle = 0;
    m_linearDamping = 0;

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
        PhysicsManager::getInstance()->removeBody(m_body);
    }

    QList<PhysicsComponent*> components = m_contacts.keys();

    foreach (PhysicsComponent* component, components)
    {
        if (component)
            component->leaveContact(this);
    }
}

QSet<QString> PhysicsComponent::getEditProperties()
{
    QSet<QString> properties;
    properties << "physicsType" << "staticRotation" << "xVelocity" << "yVelocity" << "angularVelocity" << "density" << "friction" << "linearDamping" << "isSensor";
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
        PhysicsManager::getInstance()->removeBody(m_body);
    }

    b2BodyDef bodyDef;
    if (m_type == PhysicsComponent::Static)
    {
        bodyDef.type = b2_staticBody;
    }
    else if (m_type == PhysicsComponent::Dynamic)
    {
        bodyDef.type = b2_dynamicBody;
    }
    else if (m_type == PhysicsComponent::Kinematic)
    {
        bodyDef.type = b2_kinematicBody;
    }
    bodyDef.position.Set(m_parentObject->pos().x()/20.0f, m_parentObject->pos().y()/-20.0f);
    bodyDef.angle = -(m_parentObject->rotation() * (2 * 3.14159)) / 360.0;

    //Create all of the necessary fixture definitions
    QList<QPolygonF> tesselation = m_parentObject->getTessellation();
    b2FixtureDef* fixtureDefs = new b2FixtureDef[tesselation.count()];
    double centerX = m_parentObject->width()/2.0;
    double centerY = m_parentObject->height()/2.0;
    QList<b2PolygonShape*> shapes;
    for(int j=0; j<tesselation.count(); j++)
    {
        b2Vec2 *vertices = new b2Vec2[tesselation.at(j).count()];
        for (int i=0; i<tesselation.at(j).count(); i++)
        {
            vertices[i] = b2Vec2((tesselation.at(j).at(i).x()*m_parentObject->scale() - centerX)/20.0f, -1*(tesselation.at(j).at(i).y()*m_parentObject->scale() - centerY)/20.0f);
        }
        shapes.push_back(new b2PolygonShape());
        shapes.back()->Set(vertices, tesselation.at(j).count());

        b2FixtureDef fixture;
        fixture.shape = shapes.back();
        fixture.friction = m_friction;
        fixture.density = m_density;
        fixture.isSensor = m_isSensor;
        fixtureDefs[j] = fixture;
    }
    m_body = PhysicsManager::getInstance()->addBody(&bodyDef, fixtureDefs, tesselation.count(), this);
    qDeleteAll(shapes);

    m_body->SetAngularVelocity(m_vangle);
    m_body->SetFixedRotation(m_staticRotation);
    m_body->SetLinearDamping(m_linearDamping);
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

ContactType PhysicsComponent::getContactCondition()
{
    ContactType type = ContactUnknown;

    foreach(ContactType t, m_contacts.values())
    {
        type = type | t;
    }

    return type;
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
