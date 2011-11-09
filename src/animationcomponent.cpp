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
    m_completeTrigger = "";

    connect(GameCore::getInstance(), SIGNAL(timerTick()), this, SLOT(reactToTimerTick()));
}

QSet<QString> AnimationComponent::getEditProperties()
{
    QSet<QString> properties;
    properties << "timeStep" << "pixmapFiles" << "animationTrigger" <<"completeTrigger";
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
        {
            m_currentFrame = 0;
            if (!m_completeTrigger.isEmpty())
                emit sendLocalEvent(m_completeTrigger);
        }

        m_parentObject->setTemporaryPixmapFile(m_files.at(m_currentFrame));

        this->getParentObject()->update();

        m_currentFrame++;
        m_elapsedTime = 0;
    }
}
