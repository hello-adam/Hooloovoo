#include "physicsmanager.h"

PhysicsManager* PhysicsManager::m_instance;

PhysicsManager::PhysicsManager(QObject *parent) :
    QObject(parent)
{
    m_world = new b2World(b2Vec2(0.0f, -10.0f));

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
