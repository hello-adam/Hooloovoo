#include "animationcomponent.h"
#include "gamecore.h"

AnimationComponent::AnimationComponent(GameObject *parentObject) :
    Component(parentObject)
{
    this->setObjectName("Animation Component");
    qRegisterMetaType<AnimationComponent::AnimationTrigger>("AnimationTrigger");

    m_timeStep = 60;
    m_trigger = AnimationComponent::Constant;
    m_files = QStringList();
    m_elapsedTime = 0;
    m_currentFrame = 0;

    connect(GameCore::getInstance(), SIGNAL(timerTick()), this, SLOT(reactToTimerTick()));
}

QSet<QString> AnimationComponent::getEditProperties()
{
    QSet<QString> properties;
    properties << "timeStep" << "pixmapFiles" << "animationTrigger";
    return properties;
}

void AnimationComponent::reactToTimerTick()
{
    if (m_files.count() < 1)
        return;

    m_elapsedTime++;

    if (m_elapsedTime >= m_timeStep)
    {
        if (m_currentFrame >= m_files.count())
            m_currentFrame = 0;

        m_parentObject->setPixmapFile(m_files.at(m_currentFrame));

        m_currentFrame++;
        m_elapsedTime = 0;
    }
}
