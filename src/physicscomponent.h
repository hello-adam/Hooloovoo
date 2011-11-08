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


public:
    enum BodyType { Static, Dynamic, Kinematic };

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

signals:
//    void xChanged(double x);
//    void yChanged(double y);
    void enteringContact(GameObject*);
    void leavingContact(GameObject*);

public slots:
    void setX(double x);
    void setY(double y);
    void setAngle(double degrees);

    void setBodyType(BodyType type) {m_type = type; this->instantiate();}
    void setStaticRotation(bool staticRotation) {m_staticRotation = staticRotation; this->instantiate();}
    void setSensor(bool sensor) {m_isSensor = sensor; this->instantiate();}
    void setDensity(double density) {m_density = density; this->instantiate();}
    void setVX(double vx) {m_vx = vx; this->instantiate();}
    void setVY(double vy) {m_vy = vy; this->instantiate();}
    void setVAngle(double vAngle) {m_vangle = vAngle; this->instantiate();}
    void setFriction(double friction) {m_friction = friction; this->instantiate();}
    void setLinearDamping(double damping) {if (damping < 0 || damping > 10) damping = INFINITY;  m_linearDamping = damping; this->instantiate();}

    void updateParent();
};

Q_DECLARE_METATYPE(PhysicsComponent::BodyType)

#endif // PHYSICSCOMPONENT_H
