#ifndef CONTACTCOMPONENT_H
#define CONTACTCOMPONENT_H

#include "component.h"
#include "physicscomponent.h"

class ContactComponent : public Component
{
    Q_OBJECT
    Q_PROPERTY(QString localTrigger READ getLocalTrigger WRITE setLocalTrigger)
    Q_PROPERTY(QString contactCondition READ getContactCondition WRITE setContactCondition)
    Q_PROPERTY(QString contactTrigger READ getContactTrigger WRITE setContactTrigger)

public:
    explicit ContactComponent(GameObject *parentObject);

    QSet<QString> getEditProperties();

    QString getLocalTrigger() {return m_localTrigger;}
    QString getContactCondition() {return m_contactCondition;}
    QString getContactTrigger() {return m_contactTrigger;}

private:
    PhysicsComponent* m_physicsComponent;

    QString m_localTrigger;
    QString m_contactCondition;
    QString m_contactTrigger;

signals:

public slots:
    void setLocalTrigger(QString localTrigger) {m_localTrigger = localTrigger;}
    void setContactCondition(QString contactCondition) {m_contactCondition = contactCondition;}
    void setContactTrigger(QString contactTrigger) {m_contactTrigger = contactTrigger;}

    void reactToContact(GameObject* contactObject);
    void checkForAddedPhysicsComponent(Component* c);
    void checkForRemovedPhysicsComponent(Component* c);
};

#endif // CONTACTCOMPONENT_H
