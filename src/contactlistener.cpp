#include "contactlistener.h"
#include <QDebug>

ContactListener::ContactListener(QObject *parent) :
    QObject(parent)
{
}

void ContactListener::BeginContact(b2Contact *contact)
{
    qDebug() << "New Contact";
}

void ContactListener::EndContact(b2Contact *contact)
{
    qDebug() << "End of Contact";
}
