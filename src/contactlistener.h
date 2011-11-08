#ifndef CONTACTLISTENER_H
#define CONTACTLISTENER_H

#include <Box2D/Box2D.h>
#include <QSet>

enum ContactType {ContactTop = 1,
                 ContactBottom = 2,
                 ContactLeft = 4,
                 ContactRight = 8,
                 ContactUnknown = 16};
inline ContactType operator|( ContactType lhs, ContactType rhs ) {return ContactType( int(lhs) | int(rhs) );}
inline ContactType operator&( ContactType lhs, ContactType rhs ) {return ContactType( int(lhs) & int(rhs) );}

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
