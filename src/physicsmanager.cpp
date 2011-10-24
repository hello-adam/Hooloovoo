#include "physicsmanager.h"

PhysicsManager* PhysicsManager::m_instance;

PhysicsManager::PhysicsManager(QObject *parent) :
    QObject(parent)
{
    m_worldBounds = 0;
    m_gravity = -10;
    m_world = new b2World(b2Vec2(0.0f, m_gravity));

    m_timer = new QTimer();
    m_timer->setInterval(1000.0 / 60.0);

    connect(m_timer, SIGNAL(timeout()), this, SLOT(takeStep()));
}

PhysicsManager::~PhysicsManager()
{
    if (m_timer)
        m_timer->deleteLater();
}


PhysicsManager* PhysicsManager::getInstance()
{
    if (!m_instance)
        m_instance = new PhysicsManager();

    return m_instance;
}


void PhysicsManager::start()
{
    m_timer->start();
}

void PhysicsManager::pause()
{
    m_timer->stop();
}

void PhysicsManager::takeStep()
{
    m_world->Step(1.0/60.0, 8, 3);
    emit stepTaken();
}

void PhysicsManager::setWorldBounds(QRectF rect)
{
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
}
