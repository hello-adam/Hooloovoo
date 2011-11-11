#ifndef PHYSICSCONTROLLERCOMPONENT_H
#define PHYSICSCONTROLLERCOMPONENT_H

#include "component.h"
#include "physicscomponent.h"
#include "gameobject.h"

class PhysicsControllerComponent : public Component
{
    Q_OBJECT
    Q_PROPERTY(QString engageTrigger READ getEngageTrigger WRITE setEngageTrigger)
    Q_PROPERTY(QString releaseTrigger READ getReleaseTrigger WRITE setReleaseTrigger)
    Q_PROPERTY(PhysicsControlType controlType READ getControlType WRITE setControlType)
    Q_ENUMS(PhysicsControlType)
//    Q_PROPERTY(ContactType requiredContact READ getRequiredContact WRITE setRequiredContact)
//    Q_ENUMS(ContactType)
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

private:
    PhysicsComponent* m_physicsComponent;

    QString m_engageTrigger;
    QString m_releaseTrigger;
    QPointF m_value;
    PhysicsControlType m_controlType;

signals:

public slots:
    void checkForAddedPhysicsComponent(Component* c);
    void checkForRemovecPhysicsComponent(Component* c);

    void reactToLocalEvent(QString trigger);
    void reactToTimerTick();
};

Q_DECLARE_METATYPE(PhysicsControllerComponent::PhysicsControlType)
Q_DECLARE_METATYPE(ContactType)

#endif // PHYSICSCONTROLLERCOMPONENT_H
