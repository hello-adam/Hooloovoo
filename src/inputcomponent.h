#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

#include "component.h"

class InputComponent : public Component
{
    Q_OBJECT

public:
    InputComponent(GameObject* parentObject);
};

#endif // INPUTCOMPONENT_H
