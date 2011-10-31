#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include <QObject>
#include <Box2D/Box2D.h>
#include <QTimer>
#include "physicscomponent.h"

class PhysicsManager : public QObject
{
    Q_OBJECT
public:
    static PhysicsManager* getInstance();
    ~PhysicsManager();

    b2World* getWorld() {return m_world;}

    void setGravity(double gravity) {m_gravity = gravity;  m_world->SetGravity(b2Vec2(0, m_gravity));}
    double getGravity() {return m_gravity;}

private:
    PhysicsManager(QObject* parent = 0);
    static PhysicsManager* m_instance;

    b2World *m_world;
    b2Body *m_worldBounds;

    double m_gravity;

public slots:
    void start();
    void pause();
    void takeStep();
    void setWorldBounds(QRectF rect);

signals:
    void stepTaken();
};

#endif // PHYSICSMANAGER_H
