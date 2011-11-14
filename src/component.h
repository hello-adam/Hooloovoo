#ifndef COMPONENT_H
#define COMPONENT_H

#include <QtXml>
#include <QSharedPointer>
#include <QKeyEvent>
class GameObject;

class Component : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString tag READ getTag WRITE setTag)

public:
    Component(GameObject* parentObject);

    bool deserialize(const QDomElement & specs);
    QDomElement serialize(QDomDocument *doc);

    GameObject* getParentObject() {return m_parentObject;}

    void setTag(QString tag) {m_tag = tag;}
    QString getTag() {return m_tag;}

    virtual QSet<QString> getEditProperties() {return QSet<QString>();}
    virtual void prepareForSerialization() {}
    virtual bool allowMultipleComponents() {return true;}
    virtual void instantiate() {}

protected:
    GameObject* m_parentObject;
    QString m_tag;

signals:
    void sendLocalEvent(QString);
    void sendGlobalEvent(QString);

public slots:
    virtual void checkForPropertyChange(QString trigger);
};

#endif // COMPONENT_H
