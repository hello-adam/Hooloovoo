#include "componentfactory.h"

#include "gameobject.h"
#include "physicscomponent.h"
#include "physicscontrollercomponent.h"
#include "spawncomponent.h"
#include "animationcomponent.h"
#include "inputcomponent.h"
#include "contactcomponent.h"
#include "timercomponent.h"
#include "audiocomponent.h"
#include "commandcomponent.h"

ComponentFactory::ComponentFactory()
{
}

Component* ComponentFactory::createComponent(GameObject* parentObject, QString name)
{
    Component* component;

    if (name == "Physics Component")
    {
        component = new PhysicsComponent(parentObject);
    }
    else if (name == "Physics Controller Component")
    {
        component = new PhysicsControllerComponent(parentObject);
    }
    else if (name == "Spawn Component")
    {
        component = new SpawnComponent(parentObject);
    }
    else if (name == "Animation Component")
    {
        component = new AnimationComponent(parentObject);
    }
    else if (name == "Input Component")
    {
        component = new InputComponent(parentObject);
    }
    else if (name == "Contact Component")
    {
        component = new ContactComponent(parentObject);
    }
    else if (name == "Timer Component")
    {
        component = new TimerComponent(parentObject);
    }
    else if (name == "Audio Component")
    {
        component = new AudioComponent(parentObject);
    }
    else if (name == "Command Component")
    {
        component = new CommandComponent(parentObject);
    }
    else
        return 0;

    return component;
}

Component* ComponentFactory::createComponent(GameObject* parentObject, const QDomElement & specs)
{
    QString name = specs.attribute("name");
    Component * component = createComponent(parentObject, name);

    if (!component)
        return 0;

    component->deserialize(specs);
    return component;
}

QStringList ComponentFactory::availableComponents()
{
    QStringList available;
    available << "Physics Component" << "Physics Controller Component" << "Spawn Component"
              << "Animation Component" << "Input Component" <<"Contact Component" << "Timer Component"
              << "Audio Component" << "Command Component";
    return available;
}
