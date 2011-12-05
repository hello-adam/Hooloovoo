#include "commandcomponent.h"

CommandComponent::CommandComponent()
{
    m_command = GameCore::ChangeLevel;
    m_parameter = "";
}

void CommandComponent::effectSendCommand()
{

}
