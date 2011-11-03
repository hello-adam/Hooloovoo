#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

#include "component.h"

class InputComponent : public Component
{
    Q_OBJECT
    Q_PROPERTY(Qt::Key key READ getKey WRITE setKey)
    Q_ENUMS(Qt::Key)
    Q_PROPERTY(QString trigger READ getTrigger WRITE setTrigger)
    Q_PROPERTY(TriggerType triggerType READ getType WRITE setType)
    Q_ENUMS(TriggerType)

public:
    enum TriggerType { Local, Global };

    InputComponent(GameObject* parentObject);

    QSet<QString> getEditProperties();
    void keyPressEvent(QKeyEvent* ke);
    void keyReleaseEvent(QKeyEvent* ke);

    Qt::Key getKey() {return m_key;}
    QString getTrigger() {return m_trigger;}
    TriggerType getType() {return m_type;}

private:
    Qt::Key m_key;
    QString m_trigger;
    TriggerType m_type;
    QSet<Qt::Key> m_pressed;

public slots:
    void setKey(Qt::Key key) {m_key = key;}
    void setTrigger(QString trigger) {m_trigger = trigger;}
    void setType(TriggerType type) {m_type = type;}
};

Q_DECLARE_METATYPE(Qt::Key)
Q_DECLARE_METATYPE(InputComponent::TriggerType)

#endif // INPUTCOMPONENT_H
