#ifndef PHYSICSCONTROLLERCOMPONENT_H
#define PHYSICSCONTROLLERCOMPONENT_H

#include "component.h"
#include "physicscomponent.h"
#include "gameobject.h"
#include "contactlistener.h"

class PhysicsControllerComponent : public Component
{
    Q_OBJECT
    Q_PROPERTY(QString triggerToEngage READ getEngageTrigger WRITE setEngageTrigger)
    Q_PROPERTY(QString triggerToRelease READ getReleaseTrigger WRITE setReleaseTrigger)
    Q_PROPERTY(PhysicsControlType controlType READ getControlType WRITE setControlType)
    Q_ENUMS(PhysicsControlType)
    Q_PROPERTY(PhysicsComponent::ContactType requiredContact READ getRequiredContact WRITE setRequiredContact)
    Q_PROPERTY(QPointF value READ getValue WRITE setValue)

public:
    enum PhysicsControlType { ConstantVelocity, ConstantAcceleration, Impulse };

    explicit PhysicsControllerComponent(GameObject *parentObject);
    ~PhysicsControllerComponent();

    QSet<QString> getEditProperties();

    void setEngageTrigger(QString trigger) {m_engageTrigger = trigger;}
    QString getEngageTrigger() {return m_engageTrigger;}

    void setReleaseTrigger(QString trigger) {m_releaseTrigger = trigger;}
    QString getReleaseTrigger() {return m_releaseTrigger;}

    void setValue(QPointF value) {m_value = value;}
    QPointF getValue() {return m_value;}

    void setControlType(PhysicsControlType type) {m_controlType = type;}
    PhysicsControlType getControlType() {return m_controlType;}

    void setRequiredContact(PhysicsComponent::ContactType type) {m_requiredContact = type;}
    PhysicsComponent::ContactType getRequiredContact() {return m_requiredContact;}

private:
    PhysicsComponent* m_physicsComponent;

    QString m_engageTrigger;
    QString m_releaseTrigger;
    QPointF m_value;
    PhysicsControlType m_controlType;
    PhysicsComponent::ContactType m_requiredContact;

    void reactToTrigger(QString trigger);

signals:

public slots:
    void checkForAddedPhysicsComponent(Component* c);
    void checkForRemovedPhysicsComponent(Component* c);

    void reactToTimerTick();
};

Q_DECLARE_METATYPE(PhysicsControllerComponent::PhysicsControlType)

#endif // PHYSICSCONTROLLERCOMPONENT_H
