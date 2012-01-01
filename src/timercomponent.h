#ifndef TIMERCOMPONENT_H
#define TIMERCOMPONENT_H

#include "component.h"

class TimerComponent : public Component
{
    Q_OBJECT
    Q_PROPERTY(int timeStep READ getTimeStep WRITE setTimeStep)
    Q_PROPERTY(bool activeByDefault READ getActiveByDefault WRITE setActiveByDefault)

public:
    TimerComponent(GameObject* parentObject);

    QSet<QString> getEditProperties();

    void setTimeStep(int time) {m_timeStep = time;}
    int getTimeStep() {return m_timeStep;}

    void setActiveByDefault(bool time);
    bool getActiveByDefault() {return m_activeByDefault;}

private:
    int m_timeStep;
    bool m_activeByDefault;

    int m_currentStep;

signals:
    void causeStepElapsed();

public slots:
    void effectStartTimer();
    void effectStopTimer();

    void reactToTimerTick();
};

#endif // TIMERCOMPONENT_H
