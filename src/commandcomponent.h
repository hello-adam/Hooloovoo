#ifndef COMMANDCOMPONENT_H
#define COMMANDCOMPONENT_H

#include "component.h"
#include <phonon>
#include "gamecore.h"

class CommandComponent : public Component
{
    Q_OBJECT
    Q_PROPERTY(GameCore::Command command READ getCommand WRITE setCommand)
    Q_PROPERTY(QString parameter READ getParameter WRITE setParameter)

public:
    CommandComponent(GameObject *parentObject);
    ~CommandComponent();

    QSet<QString> getEditProperties();

    GameCore::Command getCommand() {return m_command;}
    QString getParameter() {return m_parameter;}

    void setCommand(GameCore::Command command) {m_command = command;}
    void setParameter(QString parameter) {m_parameter = parameter;}

private:
    GameCore::Command m_command;
    QString m_parameter;

signals:

public slots:
    void effectSendCommand();
};

#endif // COMMANDCOMPONENT_H
