#ifndef SPAWNCOMPONENT_H
#define SPAWNCOMPONENT_H

#include "component.h"

class SpawnComponent : public Component
{
    Q_OBJECT
    Q_PROPERTY(QString objectFile READ getObjectFile WRITE setObjectFile)
    Q_PROPERTY(int spawnInterval READ getInterval WRITE setInterval)
    Q_PROPERTY(QString spawnTrigger READ getTrigger WRITE setTrigger)
    Q_PROPERTY(double xOffset READ getXOffset WRITE setXOffset)
    Q_PROPERTY(double yOffset READ getYOffset WRITE setYOffset)

public:
    SpawnComponent(GameObject *parentObject);

    QSet<QString> getEditProperties();

    QString getObjectFile() {return m_objectFileName;}
    int getInterval() {return m_interval;}
    QString getTrigger() {return m_trigger;}
    double getXOffset() {return m_offset.x();}
    double getYOffset() {return m_offset.y();}

private:
    QString m_objectFileName;
    int m_interval;
    int m_elapsed;
    QString m_trigger;
    QPointF m_offset;

    void spawn();

public slots:
    void reactToTimerTick();
    void reactToTrigger(QString trigger);

    void setObjectFile(QString file) {m_objectFileName = file;}
    void setInterval(int interval) {m_interval = interval;}
    void setTrigger(QString trigger) {m_trigger = trigger;}
    void setXOffset(double x) {m_offset.setX(x);}
    void setYOffset(double y) {m_offset.setY(y);}
};

#endif // SPAWNCOMPONENT_H
