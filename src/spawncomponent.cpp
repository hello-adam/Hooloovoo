#include "spawncomponent.h"
#include "gamecore.h"

SpawnComponent::SpawnComponent(GameObject *parentObject) :
    Component(parentObject)
{
    this->setObjectName("Spawn Component");

    m_objectFileName = "";
    m_interval = 0;
    m_elapsed = 0;
}

void SpawnComponent::reactToTimer(int timeElapsed)
{
    m_elapsed += timeElapsed;

    if (m_elapsed > m_interval)
    {
        GameCore::getInstance()->addObjectToScene(m_objectFileName, m_parentObject->pos());
        m_elapsed = 0;
    }
}

QSet<QString> SpawnComponent::getEditProperties()
{
    QSet<QString> properties;
    properties << "objectFile" << "spawnInterval";
    return properties;
}
