#include "physicsmanager.h"
#include "gamecore.h"
#include <QDebug>

PhysicsManager::PhysicsManager(QObject *parent) :
    QObject(parent)
{
    m_gravity = -42;
    m_world = new b2World(b2Vec2(0.0f, m_gravity));
    m_contactListener = new ContactListener();
    m_world->SetContactListener(m_contactListener);

    setWorldBounds(QRectF(0, 0, 800, 600));
}

PhysicsManager::~PhysicsManager()
{
    m_contactListener->deleteLater();
}

b2Body* PhysicsManager::addBody(b2BodyDef *bodyDef, b2FixtureDef fixtureDef[], int fixtureCount, PhysicsComponent *component)
{
    b2Body* body = m_world->CreateBody(bodyDef);
    m_bodyToComponent.insert(body, component);

    for(int i=0; i<fixtureCount; i++)
    {
        b2FixtureDef def = fixtureDef[i];
        b2Fixture* fixture = body->CreateFixture(&def);
        m_fixtureToComponent.insert(fixture, component);
    }

    return body;
}

bool PhysicsManager::removeBody(b2Body *body)
{
    if (!m_bodyToComponent.keys().contains(body))
    {
        qDebug() << "cannot find body in world, it cannot be removed!";
        return false;
    }

    b2Fixture* fixture = body->GetFixtureList();
    while (fixture)
    {
        m_fixtureToComponent.remove(fixture);
        fixture = fixture->GetNext();
    }

    m_world->DestroyBody(body);
    m_bodyToComponent.remove(body);
    return true;
}

void PhysicsManager::takeStep()
{
    m_world->Step(1.0/60.0, 8, 3);
    emit stepTaken();
}

void PhysicsManager::setWorldBounds(QRectF rect)
{
    m_boundingRect = rect;

    //adjust for world coordinates
    rect.setHeight(rect.height()/-20.0);
    rect.setY(rect.y()/-20.0);
    rect.setWidth(rect.width()/20.0);
    rect.setX(rect.x()/20.0);

    //delete any old world bounding chain
    if (m_worldBounds)
        m_world->DestroyBody(m_worldBounds);

    //add new world bounding chain
    b2BodyDef bd;
    m_worldBounds = m_world->CreateBody(&bd);

    b2Vec2 vs[4];
    vs[0].Set(rect.left(), rect.top());
    vs[1].Set(rect.left(), rect.bottom());
    vs[2].Set(rect.right(), rect.bottom());
    vs[3].Set(rect.right(), rect.top());

    b2ChainShape bounds;
    bounds.CreateLoop(vs, 4);

    m_worldBounds->CreateFixture(&bounds, 0.0f);

    emit boundsChanged();
}
