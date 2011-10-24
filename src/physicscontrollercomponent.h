#ifndef PHYSICSCONTROLLERCOMPONENT_H
#define PHYSICSCONTROLLERCOMPONENT_H

#include "component.h"
#include "physicscomponent.h"
#include "gameobject.h"

class PhysicsControllerComponent : public Component
{
    Q_OBJECT
public:
    explicit PhysicsControllerComponent(GameObject *parentObject);
    ~PhysicsControllerComponent();

    QSet<QString> getEditProperties();

protected:
    void keyPressEvent(QKeyEvent *ke);

private:
    PhysicsComponent* m_physicsComponent;

signals:

public slots:

};

#endif // PHYSICSCONTROLLERCOMPONENT_H
