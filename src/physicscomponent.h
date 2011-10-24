#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "component.h"
class GameObject;
#include <Box2D/Box2D.h>

class PhysicsComponent : public Component
{
    Q_OBJECT
    Q_PROPERTY(bool isDynamic READ getIsDynamic WRITE setIsDynamic)
    Q_PROPERTY(double density READ getDensity WRITE setDensity)
    Q_PROPERTY(double xVelocity READ getVX WRITE setVX)
    Q_PROPERTY(double yVelocity READ getVY WRITE setVY)
    Q_PROPERTY(double angularVelocity READ getVAngle WRITE setVAngle)
    Q_PROPERTY(double friction READ getFriction WRITE setFriction)


public:
    explicit PhysicsComponent(GameObject *parentObject);
    ~PhysicsComponent();

    QSet<QString> getEditProperties();

    void prepareForSerialization();
    void instantiate();

    bool getIsDynamic() {return m_isDynamic;}
    double getDensity() {return m_density;}
    double getVX() {return m_vx;}
    double getVY() {return m_vy;}
    double getVAngle() {return m_vangle;}
    double getFriction() {return m_friction;}

    b2Body* getBody() {return m_body;}

private:
    b2Body* m_body;

    //these are locked with the parent object, so they are not saved
    double m_x;
    double m_y;
    double m_angle;

    //these are independent properties, so they are saved
    bool m_isDynamic;
    double m_density;
    double m_vx;
    double m_vy;
    double m_vangle;
    double m_friction;

signals:
//    void xChanged(double x);
//    void yChanged(double y);

public slots:
    void setX(double x);
    void setY(double y);
    void setAngle(double degrees);

    void setIsDynamic(bool dynamic) {m_isDynamic = dynamic; this->instantiate();}
    void setDensity(double density) {m_density = density; this->instantiate();}
    void setVX(double vx) {m_vx = vx; this->instantiate();}
    void setVY(double vy) {m_vy = vy; this->instantiate();}
    void setVAngle(double vAngle) {m_vangle = vAngle; this->instantiate();}
    void setFriction(double friction) {m_friction = friction; this->instantiate();}

    void updateParent();
};

#endif // PHYSICSCOMPONENT_H
