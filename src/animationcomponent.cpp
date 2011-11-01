#include "animationcomponent.h"

AnimationComponent::AnimationComponent(GameObject *parentObject) :
    Component(parentObject)
{
    this->setObjectName("Animation Component");

    m_timeStep = 60;
    m_trigger = AnimationComponent::Constant;
    m_files = QList<QVariant>();
}

QSet<QString> AnimationComponent::getEditProperties()
{
    QSet<QString> properties;
    properties << "timeStep" << "pixmapFiles" << "animationTrigger";
    return properties;
}
