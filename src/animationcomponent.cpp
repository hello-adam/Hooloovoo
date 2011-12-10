#include "animationcomponent.h"
#include "gamecore.h"

AnimationComponent::AnimationComponent(GameObject *parentObject) :
    Component(parentObject)
{
    this->setObjectName("Animation Component");

    m_activeByDefault = false;
    m_timeStep = 10;
    m_files = QStringList();
    m_elapsedTime = 0;
    m_currentFrame = 0;
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
            emit causeAnimationComplete();
        }

        m_parentObject->setTemporaryPixmapFile(m_files.at(m_currentFrame));

        this->getParentObject()->update();

        m_currentFrame++;
        m_elapsedTime = 0;
    }
}

void AnimationComponent::effectStartAnimation()
{
    connect(&GameCore::getInstance(), SIGNAL(timerTick()), this, SLOT(reactToTimerTick()));
}

void AnimationComponent::effectStopAnimation()
{
    disconnect(&GameCore::getInstance(), SIGNAL(timerTick()), this, SLOT(reactToTimerTick()));
}
