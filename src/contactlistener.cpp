#include "contactlistener.h"
#include "physicsmanager.h"
#include "physicscomponent.h"
#include <QDebug>

ContactListener::ContactListener(QObject *parent) :
    QObject(parent)
{
}

void ContactListener::BeginContact(b2Contact *contact)
{
    b2WorldManifold worldManifold;
    contact->GetWorldManifold(&worldManifold);

    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();
    PhysicsComponent* componentA = PhysicsManager::getInstance().getComponent(fixtureA);
    PhysicsComponent* componentB = PhysicsManager::getInstance().getComponent(fixtureB);
    float32 x = worldManifold.normal.x;
    float32 y = worldManifold.normal.y;
    PhysicsComponent::ContactType typeA = PhysicsComponent::ContactIrrelevant;
    PhysicsComponent::ContactType typeB = PhysicsComponent::ContactIrrelevant;

    if (!(fixtureA->IsSensor() || fixtureB->IsSensor()))
    {
        if (x<0)
        {
            typeA = typeA | PhysicsComponent::ContactLeft;
            typeB = typeB | PhysicsComponent::ContactRight;
        }
        if (x>0)
        {
            typeA = typeA | PhysicsComponent::ContactRight;
            typeB = typeB | PhysicsComponent::ContactLeft;
        }
        if (y<0)
        {
            typeA = typeA | PhysicsComponent::ContactBottom;
            typeB = typeB | PhysicsComponent::ContactTop;
        }
        if (y>0)
        {
            typeA = typeA | PhysicsComponent::ContactTop;
            typeB = typeB | PhysicsComponent::ContactBottom;
        }
    }

    if (componentA)
        componentA->enterContact(componentB, typeA);
    if (componentB)
        componentB->enterContact(componentA, typeB);
}

void ContactListener::EndContact(b2Contact *contact)
{
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();
    PhysicsComponent* componentA = PhysicsManager::getInstance().getComponent(fixtureA);
    PhysicsComponent* componentB = PhysicsManager::getInstance().getComponent(fixtureB);

    if (componentA)
        componentA->leaveContact(componentB);
    if (componentB)
        componentB->leaveContact(componentA);
}
