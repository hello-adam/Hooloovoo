#include "timercomponent.h"
#include "gamecore.h"

TimerComponent::TimerComponent(GameObject *parentObject) :
    Component(parentObject)
{
    this->setObjectName("Timer Component");

    m_timeStep = 1;
    m_currentStep = 0;
    m_activeByDefault = false;
}

QSet<QString> TimerComponent::getEditProperties()
{
    QSet<QString> properties;
    properties << "timeStep" << "activeByDefault" << "tag";
    return properties;
}

void TimerComponent::setActiveByDefault(bool activeDefault)
{
    m_activeByDefault = activeDefault;

    if (m_activeByDefault)
        connect(&GameCore::getInstance(), SIGNAL(timerTick()), this, SLOT(reactToTimerTick()));
    else
        disconnect(&GameCore::getInstance(), SIGNAL(timerTick()), this, SLOT(reactToTimerTick()));
}

void TimerComponent::reactToTimerTick()
{
    m_currentStep += 1;

    if (m_currentStep >= m_timeStep)
    {
        emit causeStepElapsed();
        m_currentStep = 0;
    }
}

void TimerComponent::effectStartTimer()
{
    connect(&GameCore::getInstance(), SIGNAL(timerTick()), this, SLOT(reactToTimerTick()));
}

void TimerComponent::effectStopTimer()
{
    disconnect(&GameCore::getInstance(), SIGNAL(timerTick()), this, SLOT(reactToTimerTick()));
}
