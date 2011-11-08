#include "componentfactory.h"

#include "gameobject.h"
#include "physicscomponent.h"
#include "physicscontrollercomponent.h"
#include "spawncomponent.h"
#include "animationcomponent.h"
#include "inputcomponent.h"
#include "contactcomponent.h"

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
    else if (name == "Spawn Component")
    {
        SpawnComponent* spawn = new SpawnComponent(parentObject);
        return spawn;
    }
    else if (name == "Animation Component")
    {
        AnimationComponent* anim = new AnimationComponent(parentObject);
        return anim;
    }
    else if (name == "Input Component")
    {
        InputComponent* input = new InputComponent(parentObject);
        return input;
    }
    else if (name == "Contact Component")
    {
        ContactComponent* contact = new ContactComponent(parentObject);
        return contact;
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
    else if (name == "Spawn Component")
    {
        SpawnComponent* spawn = new SpawnComponent(parentObject);
        spawn->deserialize(specs);
        return spawn;
    }
    else if (name == "Animation Component")
    {
        AnimationComponent* anim = new AnimationComponent(parentObject);
        anim->deserialize(specs);
        return anim;
    }
    else if (name == "Input Component")
    {
        InputComponent* input = new InputComponent(parentObject);
        input->deserialize(specs);
        return input;
    }
    else if (name == "Contact Component")
    {
        ContactComponent* contact = new ContactComponent(parentObject);
        contact->deserialize(specs);
        return contact;
    }
    else
        return 0;
}

QStringList ComponentFactory::availableComponents()
{
    QStringList available;
    available << "Physics Component" << "Physics Controller Component" << "Spawn Component" << "Animation Component" << "Input Component" <<"Contact Component";
    return available;
}
