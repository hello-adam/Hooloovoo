#include "inputcomponent.h"

InputComponent::InputComponent(GameObject *parentObject) :
    Component(parentObject)
{
    this->setObjectName("Input Component");
}
