#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

#include "component.h"

class AnimationComponent : public Component
{
    Q_OBJECT
    Q_PROPERTY(int timeStep READ getTimeStep WRITE setTimeStep)
    Q_PROPERTY(AnimationTrigger animationTrigger READ getTrigger WRITE setTrigger)
    Q_ENUMS(AnimationTrigger)
    Q_PROPERTY(QStringList pixmapFiles READ getFiles WRITE setFiles)

public:
    AnimationComponent(GameObject *parentObject);

    QSet<QString> getEditProperties();

    enum AnimationTrigger {Constant, Movement, HorizontalMovement, VerticalMovement};

    int getTimeStep() {return m_timeStep;}
    AnimationTrigger getTrigger() {return m_trigger;}
    QStringList getFiles() {return m_files;}

private:
    int m_timeStep;
    QStringList m_files;
    AnimationTrigger m_trigger;

public slots:
    void setTimeStep(int timeStep) {m_timeStep = timeStep;}
    void setTrigger(AnimationTrigger trigger) {m_trigger = trigger;}
    void setFiles(QStringList files) {m_files = files;}
};

Q_DECLARE_METATYPE(AnimationComponent::AnimationTrigger)

#endif // ANIMATIONCOMPONENT_H
