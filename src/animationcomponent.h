#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

#include "component.h"

class AnimationComponent : public Component
{
    Q_OBJECT
    Q_PROPERTY(int timeStep READ getTimeStep WRITE setTimeStep)
    Q_PROPERTY(bool activeByDefault READ getDefault WRITE setDefault)
    Q_PROPERTY(QString triggerToStart READ getStart WRITE setStart)
    Q_PROPERTY(QString triggerToStop READ getStop WRITE setStop)
    Q_PROPERTY(QString completeTrigger READ getCompleteTrigger WRITE setCompleteTrigger)
    Q_PROPERTY(QStringList pixmapFiles READ getFiles WRITE setFiles)

public:
    AnimationComponent(GameObject *parentObject);

    QSet<QString> getEditProperties();

    int getTimeStep() {return m_timeStep;}
    QString getStart() {return m_startTrigger;}
    QString getStop() {return m_stopTrigger;}
    QStringList getFiles() {return m_files;}
    QString getCompleteTrigger() {return m_completeTrigger;}
    bool getActive() {return m_activeByDefault;}
    bool getDefault() {return m_activeByDefault;}

private:
    int m_timeStep;
    QString m_completeTrigger;
    QStringList m_files;
    QString m_startTrigger;
    QString m_stopTrigger;
    int m_currentFrame;
    int m_elapsedTime;
    bool m_activeByDefault;

    void reactToTrigger(QString trigger);

public slots:
    void setTimeStep(int timeStep) {m_timeStep = timeStep;}
    void setCompleteTrigger(QString trigger) {m_completeTrigger = trigger;}
    void setStart(QString trigger) {m_startTrigger = trigger;}
    void setStop(QString trigger) {m_stopTrigger = trigger;}
    void setFiles(QStringList files) {m_files = files;}
    void setDefault(bool active);

    void reactToTimerTick();
};

#endif // ANIMATIONCOMPONENT_H
