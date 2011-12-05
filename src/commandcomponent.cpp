#include "commandcomponent.h"

CommandComponent::CommandComponent(GameObject *parentObject) :
    Component(parentObject)
{
    m_command = GameCore::ChangeLevel;
    m_parameter = "";
}

CommandComponent::~CommandComponent()
{

}

QSet<QString> CommandComponent::getEditProperties()
{
    QSet<QString> properties;
    properties << "tag" << "command" << "parameter";
    return properties;
}

void CommandComponent::effectSendCommand()
{

}
