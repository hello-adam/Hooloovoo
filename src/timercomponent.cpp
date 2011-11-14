#include "timercomponent.h"
#include "gamecore.h"

TimerComponent::TimerComponent(GameObject *parentObject) :
    Component(parentObject)
{
    this->setObjectName("Timer Component");

    m_isInitiated = false;
    m_initiate = "";
    m_stop = "";
    m_step = "";
    m_timeStep = 1;
    m_currentStep = 0;

    connect(GameCore::getInstance(), SIGNAL(timerTick()), this, SLOT(reactToTimerTick()));
    connect(parentObject, SIGNAL(sendLocalEvent(QString)), this, SLOT(reactToTrigger(QString)));
}

QSet<QString> TimerComponent::getEditProperties()
{
    QSet<QString> properties;
    properties << "timeStep" << "initiateTrigger" << "stopTrigger" << "stepTrigger";
    return properties;
}

void TimerComponent::reactToTimerTick()
{
    if (!m_isInitiated)
        return;

    m_currentStep += 1;

    if (m_currentStep >= m_timeStep)
    {
        emit sendLocalEvent(m_step);
        m_currentStep = 0;
    }
}

void TimerComponent::reactToTrigger(QString trigger)
{
    if (trigger == m_initiate)
    {
        m_isInitiated = true;
    }
    if (trigger == m_stop)
    {
        m_isInitiated = false;
    }
}
