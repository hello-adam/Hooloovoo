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
    QDomElement serialize();

    GameObject* getParentObject();

    void setTag(QString tag) {m_tag = tag;}
    QString getTag() {return m_tag;}

    virtual QSet<QString> getEditProperties() {return QSet<QString>();}
    virtual void prepareForSerialization() {}
    virtual bool allowMultipleComponents() {return true;}

    QStringList getCauseList();
    QStringList getEffectList();

    int getID() {return m_ID;}

protected:
    GameObject* m_parentObject;
    QString m_tag;
    int m_ID;

    virtual void privateSerialize(QDomElement & componentObject) {Q_UNUSED(componentObject)}
    virtual void privateDeserialize(const QDomElement & componentObject) {Q_UNUSED(componentObject)}

signals:

public slots:
    virtual void instantiate() {}
};

#endif // COMPONENT_H
