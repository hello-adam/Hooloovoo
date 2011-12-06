#ifndef CONTACTCOMPONENT_H
#define CONTACTCOMPONENT_H

#include "component.h"
#include "physicscomponent.h"

class ContactComponent : public Component
{
    Q_OBJECT

public:
    explicit ContactComponent(GameObject *parentObject);

    QSet<QString> getEditProperties();

private:
    PhysicsComponent* m_physicsComponent;
    bool m_madeContact;

signals:
    void causeEnterContact();

public slots:

    void reactToContact(GameObject* contactObject);
    void checkForAddedPhysicsComponent(Component* c);
    void checkForRemovedPhysicsComponent(Component* c);
    void contactCheck();
};

#endif // CONTACTCOMPONENT_H
