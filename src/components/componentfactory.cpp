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

QString ComponentFactory::componentInfo(QString name)
{
        if (name == "Physics Component")
        {
            return "The Physics Component allows the object to interact with other objects with Physics Components as though they were objects in "
                    "the physical world.  This means that they can realistically apply forces on each other with respect to the physical properties that you assign to the Physics Component.";
        }
        else if (name == "Physics Controller Component")
        {
            return "The Physics Controller Component allows the object's Physics Component to be smoothly controlled.";
        }
        else if (name == "Spawn Component")
        {
            return "The Spawn Component allows the object to create/spawn other objects in the game.";
        }
        else if (name == "Animation Component")
        {
            return "The Animation Component can be used to make a game object's appearance animated (rather than having it as a static picture).  "
                    "Just select the frames of the animation in their appropriate order and arrange for the the animation to be started and stopped "
                    "by causes within the object.";
        }
        else if (name == "Input Component")
        {
            return "The Input Component allows you to map external inputs to causes in an object that can trigger effects within that object.";
        }
        else if (name == "Contact Component")
        {
            return "The Contact Component allows the object to monitor its Physics Component and trigger causes (which canbe mapped to effects) when the object "
                    "makes contact with another Physics Component.";
        }
        else if (name == "Timer Component")
        {
            return "The Timer Component gives the object a timer that can be started and stopped by internal causes, and will trigger a cause whenever it is activated for the specified time step.";
        }
        else if (name == "Audio Component")
        {
            return "The Audio component gives the object the ability to play sounds in response to internal causes.";
        }
        else if (name == "Command Component")
        {
            return "The Command Component gives the object the ability to issue commands (such as a level change demand) to the central game core.";
        }
        else
            return "";
}
