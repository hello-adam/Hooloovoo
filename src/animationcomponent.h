#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

#include "component.h"

class AnimationComponent : public Component
{
    Q_OBJECT
    Q_PROPERTY(int timeStep READ getTimeStep WRITE setTimeStep)
    Q_PROPERTY(bool activeByDefault READ getDefault WRITE setDefault)
    Q_PROPERTY(QStringList pixmapFiles READ getFiles WRITE setFiles)

public:
    AnimationComponent(GameObject *parentObject);

    QSet<QString> getEditProperties()
    {
        QSet<QString> properties;
        properties << "timeStep" << "pixmapFiles" << "activeByDefault";
        return properties;
    }

    int getTimeStep() {return m_timeStep;}
    QStringList getFiles() {return m_files;}
    bool getDefault() {return m_activeByDefault;}

private:
    int m_timeStep;
    QStringList m_files;
    int m_currentFrame;
    int m_elapsedTime;
    bool m_activeByDefault;

signals:
    void causeAnimationComplete();

public slots:
    void setTimeStep(int timeStep) {m_timeStep = timeStep;}
    void setFiles(QStringList files) {m_files = files;}
    void setDefault(bool active);

    void effectStartAnimation();
    void effectStopAnimation();

    void reactToTimerTick();
};

#endif // ANIMATIONCOMPONENT_H
