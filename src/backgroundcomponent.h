#ifndef BACKGROUNDCOMPONENT_H
#define BACKGROUNDCOMPONENT_H

#include "component.h"

class BackgroundComponent : public Component
{
public:
    BackgroundComponent(GameObject* parentObject);

//    bool deserialize(const QDomElement & specs) {}
//    QDomElement serialize(QDomDocument *doc) {}
};

#endif // BACKGROUNDCOMPONENT_H
