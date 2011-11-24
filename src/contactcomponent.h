#ifndef CONTACTCOMPONENT_H
#define CONTACTCOMPONENT_H

#include "component.h"
#include "physicscomponent.h"

class ContactComponent : public Component
{
    Q_OBJECT
    Q_PROPERTY(QString thisObjectTrigger READ getLocalTrigger WRITE setLocalTrigger)
    Q_PROPERTY(QString contactedObjectTrigger READ getContactTrigger WRITE setContactTrigger)

public:
    explicit ContactComponent(GameObject *parentObject);

    QSet<QString> getEditProperties();

    QString getLocalTrigger() {return m_localTrigger;}
    QString getContactTrigger() {return m_contactTrigger;}

private:
    PhysicsComponent* m_physicsComponent;

    QString m_localTrigger;
    QString m_contactTrigger;

signals:

public slots:
    void setLocalTrigger(QString localTrigger) {m_localTrigger = localTrigger;}
    void setContactTrigger(QString contactTrigger) {m_contactTrigger = contactTrigger;}

    void reactToContact(GameObject* contactObject);
    void checkForAddedPhysicsComponent(Component* c);
    void checkForRemovedPhysicsComponent(Component* c);
};

#endif // CONTACTCOMPONENT_H
