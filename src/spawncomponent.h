#ifndef SPAWNCOMPONENT_H
#define SPAWNCOMPONENT_H

#include "component.h"

class SpawnComponent : public Component
{
    Q_OBJECT
    Q_PROPERTY(QString objectFile READ getObjectFile WRITE setObjectFile)
    Q_PROPERTY(int spawnInterval READ getInterval WRITE setInterval)

public:
    SpawnComponent(GameObject *parentObject);

    QSet<QString> getEditProperties();

    QString getObjectFile() {return m_objectFileName;}
    int getInterval() {return m_interval;}

private:
    QString m_objectFileName;
    int m_interval;
    int m_elapsed;

public slots:
    void reactToTimer(int timeElapsed);

    void setObjectFile(QString file) {m_objectFileName = file;}
    void setInterval(int interval) {m_interval = interval;}
};

#endif // SPAWNCOMPONENT_H
