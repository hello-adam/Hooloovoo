#include "animationcomponent.h"
#include "gamecore.h"

AnimationComponent::AnimationComponent(GameObject *parentObject) :
    Component(parentObject)
{
    this->setObjectName("Animation Component");

    m_timeStep = 10;
    m_startTrigger = "";
    m_stopTrigger = "";
    m_files = QStringList();
    m_elapsedTime = 0;
    m_currentFrame = 0;
    m_completeTrigger = "";
}

QSet<QString> AnimationComponent::getEditProperties()
{
    QSet<QString> properties;
    properties << "timeStep" << "pixmapFiles" << "triggerToStart" << "triggerToStop" << "completeTrigger" << "activeByDefault";
    return properties;
}

void AnimationComponent::reactToTrigger(QString trigger)
{
    if (trigger == m_startTrigger)
    {
        connect(&GameCore::getInstance(), SIGNAL(timerTick()), this, SLOT(reactToTimerTick()));
    }
    else if (trigger == m_stopTrigger)
    {
        disconnect(&GameCore::getInstance(), SIGNAL(timerTick()), this, SLOT(reactToTimerTick()));
    }
}

void AnimationComponent::setDefault(bool active)
{
    m_activeByDefault = active;

    if (active)
        connect(&GameCore::getInstance(), SIGNAL(timerTick()), this, SLOT(reactToTimerTick()));
}

void AnimationComponent::reactToTimerTick()
{
    if (m_files.count() < 1)
        return;

    m_elapsedTime++;

    if (m_elapsedTime >= m_timeStep)
    {
        if (m_currentFrame >= m_files.count())
        {
            m_currentFrame = 0;
            if (!m_completeTrigger.isEmpty())
                emit sendLocalTrigger(m_completeTrigger);
        }

        m_parentObject->setTemporaryPixmapFile(m_files.at(m_currentFrame));

        this->getParentObject()->update();

        m_currentFrame++;
        m_elapsedTime = 0;
    }
}
