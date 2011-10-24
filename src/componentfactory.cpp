#include "componentfactory.h"

#include "gameobject.h"
#include "physicscomponent.h"
#include "physicscontrollercomponent.h"
#include "backgroundcomponent.h"

ComponentFactory::ComponentFactory()
{
}

Component* ComponentFactory::createComponent(GameObject* parentObject, QString name)
{
    if (name == "Physics Component")
    {
        PhysicsComponent* physics = new PhysicsComponent(parentObject);
        physics->instantiate();
        return physics;
    }
    else if (name == "Physics Controller Component")
    {
        PhysicsControllerComponent* pControl = new PhysicsControllerComponent(parentObject);
        return pControl;
    }
    else
        return 0;
}

Component* ComponentFactory::createComponent(GameObject* parentObject, const QDomElement & specs)
{
    QString name = specs.attribute("name");

    if (name == "Physics Component")
    {
        PhysicsComponent* physics = new PhysicsComponent(parentObject);
        physics->deserialize(specs);
        physics->instantiate();
        return physics;
    }
    else if (name == "Physics Controller Component")
    {
        PhysicsControllerComponent* pControl = new PhysicsControllerComponent(parentObject);
        pControl->deserialize(specs);
        return pControl;
    }
    else
        return 0;
}

QStringList ComponentFactory::availableComponents()
{
    QStringList available;
    available << "Physics Component" << "Physics Controller Component";
    return available;
}
