#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

#include "component.h"
#include "inputreceiver.h"

class InputComponent : public Component, public InputReceiver
{
    Q_OBJECT
    Q_PROPERTY(Qt::Key key READ getKey WRITE setKey)
    Q_ENUMS(Qt::Key)
    Q_PROPERTY(QString pressTrigger READ getTrigger WRITE setTrigger)
    Q_PROPERTY(QString releaseTrigger READ getReleaseTrigger WRITE setReleaseTrigger)
    Q_PROPERTY(TriggerType triggerType READ getType WRITE setType)
    Q_ENUMS(TriggerType)

public:
    enum TriggerType { Local, Global };

    InputComponent(GameObject* parentObject);

    QSet<QString> getEditProperties();

    void handleKeyEvent(QKeyEvent *ke);
    void keyPressEvent(QKeyEvent* ke);
    void keyReleaseEvent(QKeyEvent* ke);

    Qt::Key getKey() {return m_key;}
    QString getTrigger() {return m_trigger;}
    QString getReleaseTrigger() {return m_releaseTrigger;}
    TriggerType getType() {return m_type;}

private:
    Qt::Key m_key;
    QString m_trigger;
    QString m_releaseTrigger;
    TriggerType m_type;
    QSet<Qt::Key> m_pressed;

public slots:
    void setKey(Qt::Key key) {m_key = key;}
    void setTrigger(QString trigger) {m_trigger = trigger;}
    void setReleaseTrigger(QString releaseTrigger) {m_releaseTrigger = releaseTrigger;}
    void setType(TriggerType type) {m_type = type;}
};

Q_DECLARE_METATYPE(Qt::Key)
Q_DECLARE_METATYPE(InputComponent::TriggerType)

#endif // INPUTCOMPONENT_H
