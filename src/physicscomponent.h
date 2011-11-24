#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "component.h"
class GameObject;
#include <Box2D/Box2D.h>
#include <QDebug>
#include "contactlistener.h"

class PhysicsComponent : public Component
{
    Q_OBJECT
    Q_PROPERTY(BodyType physicsType READ getBodyType WRITE setBodyType)
    Q_ENUMS(BodyType)
    Q_PROPERTY(bool staticRotation READ getStaticRotation WRITE setStaticRotation)
    Q_PROPERTY(bool isSensor READ getSensor WRITE setSensor)
    Q_PROPERTY(double density READ getDensity WRITE setDensity)
    Q_PROPERTY(double xVelocity READ getVX WRITE setVX)
    Q_PROPERTY(double yVelocity READ getVY WRITE setVY)
    Q_PROPERTY(double angularVelocity READ getVAngle WRITE setVAngle)
    Q_PROPERTY(double friction READ getFriction WRITE setFriction)
    Q_PROPERTY(double linearDamping READ getLinearDamping WRITE setLinearDamping)
    Q_PROPERTY(double x READ getX WRITE setX)
    Q_PROPERTY(double y READ getY WRITE setY)
    Q_PROPERTY(double rotation READ getAngle WRITE setAngle)

    Q_ENUMS(ContactType)


public:
    enum BodyType { Static, Dynamic, Kinematic };
    enum ContactType {  ContactTop = 1,
                        ContactBottom = 2,
                        ContactLeft = 4,
                        ContactRight = 8,
                        ContactIrrelevant = 16};


    explicit PhysicsComponent(GameObject *parentObject);
    ~PhysicsComponent();

    QSet<QString> getEditProperties();
    bool allowMultipleComponents() {return false;}

    void prepareForSerialization();
    void instantiate();

    BodyType getBodyType() {return m_type;}
    bool getStaticRotation() {return m_staticRotation;}
    bool getSensor() {return m_isSensor;}
    double getDensity() {return m_density;}
    double getVX() {return m_vx;}
    double getVY() {return m_vy;}
    double getVAngle() {return m_vangle;}
    double getFriction() {return m_friction;}
    double getLinearDamping() {return m_linearDamping;}
    b2Body* getBody() {return m_body;}
    double getX() {return m_x;}
    double getY() {return m_y;}
    double getAngle() {return m_angle;}

    void enterContact(PhysicsComponent* contact, ContactType type) {m_contacts.insert(contact, type);  if (contact) emit enteringContact(contact->getParentObject());}
    void leaveContact(PhysicsComponent* contact) {m_contacts.remove(contact);  if (contact) emit leavingContact(contact->getParentObject());}
    ContactType getContactCondition();

private:
    b2Body* m_body;
    QHash<PhysicsComponent*, ContactType> m_contacts;

    //these are locked with the parent object, so they are not saved
    double m_x;
    double m_y;
    double m_angle;

    //these are independent properties, so they are saved
    BodyType m_type;
    bool m_staticRotation;
    bool m_isSensor;
    double m_density;
    double m_vx;
    double m_vy;
    double m_vangle;
    double m_friction;
    double m_linearDamping;

    QList<QStringList> m_delayedPropertyAlterations;
    void dealWithDelayedPropertyAlterations();

    void reactToPropertyTrigger(QStringList args);

signals:
//    void xChanged(double x);
//    void yChanged(double y);
    void enteringContact(GameObject*);
    void leavingContact(GameObject*);

public slots:
    void setX(double x);
    void setY(double y);
    void setAngle(double degrees);

    void setBodyType(BodyType type) {m_type = type;}
    void setStaticRotation(bool staticRotation) {m_staticRotation = staticRotation;}
    void setSensor(bool sensor) {m_isSensor = sensor;}
    void setDensity(double density) {m_density = density;}
    void setVX(double vx) {m_vx = vx;}
    void setVY(double vy) {m_vy = vy;}
    void setVAngle(double vAngle) {m_vangle = vAngle;}
    void setFriction(double friction) {m_friction = friction;}
    void setLinearDamping(double damping) {if (damping < 0 || damping > 10) damping = INFINITY;  m_linearDamping = damping;}

    void updateParent();
};


inline PhysicsComponent::ContactType operator|( PhysicsComponent::ContactType lhs, PhysicsComponent::ContactType rhs ) {return PhysicsComponent::ContactType( int(lhs) | int(rhs) );}
inline PhysicsComponent::ContactType operator&( PhysicsComponent::ContactType lhs, PhysicsComponent::ContactType rhs ) {return PhysicsComponent::ContactType( int(lhs) & int(rhs) );}

Q_DECLARE_METATYPE(PhysicsComponent::BodyType)

#endif // PHYSICSCOMPONENT_H
