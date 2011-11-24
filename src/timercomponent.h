#ifndef TIMERCOMPONENT_H
#define TIMERCOMPONENT_H

#include "component.h"

class TimerComponent : public Component
{
    Q_OBJECT
    Q_PROPERTY(int timeStep READ getTimeStep WRITE setTimeStep)
    Q_PROPERTY(QString initiateTrigger READ getInitiate WRITE setInitiate)
    Q_PROPERTY(QString stopTrigger READ getStop WRITE setStop)
    Q_PROPERTY(QString stepTrigger READ getStep WRITE setStep)

public:
    TimerComponent(GameObject* parentObject);

    QSet<QString> getEditProperties();

    void setTimeStep(int time) {m_timeStep = time;}
    int getTimeStep() {return m_timeStep;}

    void setInitiate(QString initiate) {m_initiate = initiate;}
    QString getInitiate() {return m_initiate;}

    void setStop(QString stop) {m_stop = stop;}
    QString getStop() {return m_stop;}

    void setStep(QString step) {m_step = step;}
    QString getStep() {return m_step;}

private:
    int m_timeStep;
    QString m_initiate;
    QString m_stop;
    QString m_step;

    bool m_isInitiated;
    int m_currentStep;

    void reactToTrigger(QString trigger);

public slots:
    void reactToTimerTick();
};

#endif // TIMERCOMPONENT_H
