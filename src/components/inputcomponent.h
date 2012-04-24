#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

#include "component.h"
#include "inputreceiver.h"

class InputComponent : public Component, public InputReceiver
{
    Q_OBJECT
    Q_PROPERTY(Qt::Key key READ getKey WRITE setKey)
    Q_ENUMS(Qt::Key)

public:
    InputComponent(GameObject* parentObject);

    QSet<QString> getEditProperties();

    void handleKeyEvent(QKeyEvent *ke);
    void keyPressEvent(QKeyEvent* ke);
    void keyReleaseEvent(QKeyEvent* ke);

    Qt::Key getKey() {return m_key;}
private:
    Qt::Key m_key;
    QSet<Qt::Key> m_pressed;

signals:
    void causeKeyPressed();
    void causeKeyReleased();

public slots:
    void setKey(Qt::Key key) {m_key = key;}
};

Q_DECLARE_METATYPE(Qt::Key)

#endif // INPUTCOMPONENT_H
