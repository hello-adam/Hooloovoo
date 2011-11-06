#ifndef CONTACTLISTENER_H
#define CONTACTLISTENER_H

#include <Box2D/Box2D.h>
#include <QSet>

class ContactListener : public QObject, public b2ContactListener
{
    Q_OBJECT

public:
    ContactListener(QObject *parent = 0);

    void BeginContact(b2Contact *contact);
    void EndContact(b2Contact *contact);

private:

signals:
};

#endif // CONTACTLISTENER_H
