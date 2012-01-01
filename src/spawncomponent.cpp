#include "spawncomponent.h"
#include "gamecore.h"
#include "filemanager.h"

SpawnComponent::SpawnComponent(GameObject *parentObject) :
    Component(parentObject)
{
    this->setObjectName("Spawn Component");
    m_tag = "Spawn";

    m_objectFileName = "";
    m_interval = -1;
    m_elapsed = 0;
    m_offset = QPointF(0, 0);

    connect(&GameCore::getInstance(), SIGNAL(timerTick()), this, SLOT(reactToTimerTick()));

    m_properties << new Property(this, "objectFile", Property::GameObjectDialog);
    m_properties << new Property(this, "spawnInterval");
    m_properties << new Property(this, "xOffset");
    m_properties << new Property(this, "yOffset");
}

QSet<QString> SpawnComponent::getEditProperties()
{
    QSet<QString> properties;
    properties << "objectFile" << "spawnInterval" << "xOffset" << "yOffset" << "tag";
    return properties;
}

void SpawnComponent::spawn()
{
    GameCore::getInstance().addObjectToCurrentLevel(FileManager::getInstance().loadGameObject(m_objectFileName), m_parentObject->pos() + m_offset);
}

void SpawnComponent::effectSpawn()
{
    this->spawn();
}

void SpawnComponent::reactToTimerTick()
{
    if (m_interval < 0)
        return;

    m_elapsed += 1;

    if (m_elapsed > m_interval)
    {
        spawn();
        m_elapsed = 0;
    }
}
