#ifndef SPAWNCOMPONENT_H
#define SPAWNCOMPONENT_H

#include "component.h"

class SpawnComponent : public Component
{
    Q_OBJECT
    Q_PROPERTY(QString objectFile READ getObjectFile WRITE setObjectFile)
    Q_PROPERTY(int spawnInterval READ getInterval WRITE setInterval)
    Q_PROPERTY(QString spawnTrigger READ getTrigger WRITE setTrigger)

public:
    SpawnComponent(GameObject *parentObject);

    QSet<QString> getEditProperties();

    QString getObjectFile() {return m_objectFileName;}
    int getInterval() {return m_interval;}
    QString getTrigger() {return m_trigger;}

private:
    QString m_objectFileName;
    int m_interval;
    int m_elapsed;
    QString m_trigger;

    void spawn();

public slots:
    void reactToTimerTick();
    void reactToTrigger(QString trigger);

    void setObjectFile(QString file) {m_objectFileName = file;}
    void setInterval(int interval) {m_interval = interval;}
    void setTrigger(QString trigger) {m_trigger = trigger;}
};

#endif // SPAWNCOMPONENT_H
