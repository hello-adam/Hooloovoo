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

private:
    PhysicsManager(QObject* parent = 0);
    static PhysicsManager* m_instance;

    b2World *m_world;
    QTimer *m_timer;
    b2Body *m_worldBounds;

public slots:
    void start();
    void pause();
    void takeStep();
    void setWorldBounds(QRectF rect);

signals:
    void stepTaken();
};

#endif // PHYSICSMANAGER_H
