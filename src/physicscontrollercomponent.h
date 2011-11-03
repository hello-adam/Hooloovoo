#ifndef PHYSICSCONTROLLERCOMPONENT_H
#define PHYSICSCONTROLLERCOMPONENT_H

#include "component.h"
#include "physicscomponent.h"
#include "gameobject.h"

class PhysicsControllerComponent : public Component
{
    Q_OBJECT
    Q_PROPERTY(double jumpImpulse READ getJumpImpulse WRITE setJumpImpulse)
    Q_PROPERTY(double movementSpeed READ getMovementSpeed WRITE setMovementSpeed)

public:
    explicit PhysicsControllerComponent(GameObject *parentObject);
    ~PhysicsControllerComponent();

    QSet<QString> getEditProperties();
    bool allowMultipleComponents() {return false;}

    void setJumpImpulse(double impulse) {m_jumpImpulse = impulse;}
    double getJumpImpulse() {return m_jumpImpulse;}

    void setMovementSpeed(double speed) {m_movementSpeed = speed;}
    double getMovementSpeed() {return m_movementSpeed;}

    void keyPressEvent(QKeyEvent *ke);
    void keyReleaseEvent(QKeyEvent *ke);

private:
    PhysicsComponent* m_physicsComponent;
    double m_jumpImpulse;
    double m_movementSpeed;

    QSet<int> m_pressedKeys;

signals:
    void sendLocalEvent(QString);

public slots:
    void checkForAddedPhysicsComponent(Component* c);
    void checkForRemovecPhysicsComponent(Component* c);

};

#endif // PHYSICSCONTROLLERCOMPONENT_H
