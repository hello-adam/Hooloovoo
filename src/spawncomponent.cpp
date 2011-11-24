#include "spawncomponent.h"
#include "gamecore.h"

SpawnComponent::SpawnComponent(GameObject *parentObject) :
    Component(parentObject)
{
    this->setObjectName("Spawn Component");

    m_objectFileName = "";
    m_interval = -1;
    m_elapsed = 0;
    m_offset = QPointF(0, 0);

    connect(GameCore::getInstance(), SIGNAL(timerTick()), this, SLOT(reactToTimerTick()));
}

QSet<QString> SpawnComponent::getEditProperties()
{
    QSet<QString> properties;
    properties << "objectFile" << "spawnInterval" << "spawnTrigger" << "xOffset" << "yOffset";
    return properties;
}

void SpawnComponent::spawn()
{
    GameCore::getInstance()->addObjectToScene(m_objectFileName, m_parentObject->pos() + m_offset);
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

void SpawnComponent::reactToTrigger(QString trigger)
{
    if (trigger == m_trigger)
    {
        spawn();
    }
}
