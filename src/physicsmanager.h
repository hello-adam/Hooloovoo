#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include <QObject>
#include <Box2D/Box2D.h>
#include <QTimer>
#include "physicscomponent.h"
#include "contactlistener.h"

class PhysicsManager : public QObject
{
    Q_OBJECT
public:
    static PhysicsManager* getInstance();
    ~PhysicsManager();

    void setGravity(double gravity) {m_gravity = gravity;  m_world->SetGravity(b2Vec2(0, m_gravity));}
    double getGravity() {return m_gravity;}

    b2Body* addBody(b2BodyDef* bodyDef, b2FixtureDef fixtureDef[], int fixtureCount, PhysicsComponent* component);
    bool removeBody(b2Body* body);

    PhysicsComponent* getComponent(b2Body* body) {return m_bodyToComponent.value(body);}
    PhysicsComponent* getComponent(b2Fixture* fixture) {return m_fixtureToComponent.value(fixture);}

private:
    PhysicsManager(QObject* parent = 0);
    static PhysicsManager* m_instance;

    b2World *m_world;
    b2Body *m_worldBounds;
    ContactListener *m_contactListener;
    double m_gravity;
    QHash<b2Body*, PhysicsComponent*> m_bodyToComponent;
    QHash<b2Fixture*, PhysicsComponent*> m_fixtureToComponent;

public slots:
    void start();
    void pause();
    void takeStep();
    void setWorldBounds(QRectF rect);

signals:
    void stepTaken();
};

#endif // PHYSICSMANAGER_H
