#include "inputreceiver.h"
#include "gamecore.h"

InputReceiver::InputReceiver()
{
    GameCore::getInstance()->addInputReceiver(this);
}

InputReceiver::~InputReceiver()
{
    GameCore::getInstance()->removeInputReceiver(this);
}
