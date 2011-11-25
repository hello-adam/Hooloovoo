#ifndef AUDIOCOMPONENT_H
#define AUDIOCOMPONENT_H

#include "component.h"
#include <phonon>

class AudioComponent : public Component
{
    Q_OBJECT
    Q_PROPERTY(bool repeat READ getRepeat WRITE setRepeat)
    Q_PROPERTY(QString triggerToStart READ getStart WRITE setStart)
    Q_PROPERTY(QString triggerToStop READ getStop WRITE setStop)
    Q_PROPERTY(QString completeTrigger READ getCompleteTrigger WRITE setCompleteTrigger)
    Q_PROPERTY(QString audioFile READ getFile WRITE setFile)

public:
    AudioComponent(GameObject *parentObject);
    ~AudioComponent();

    QSet<QString> getEditProperties();

    int getTimeStep() {return m_timeStep;}
    QString getStart() {return m_startTrigger;}
    QString getStop() {return m_stopTrigger;}
    QString getFile() {return m_file;}
    QString getCompleteTrigger() {return m_completeTrigger;}
    bool getRepeat() {return m_repeat;}

private:
    int m_timeStep;
    QString m_completeTrigger;
    QString m_file;
    QString m_startTrigger;
    QString m_stopTrigger;
    bool m_repeat;
    Phonon::MediaObject *m_audioObject;
    Phonon::Path m_path;

    void reactToTrigger(QString trigger);

public slots:
    void setCompleteTrigger(QString trigger) {m_completeTrigger = trigger;}
    void setStart(QString trigger) {m_startTrigger = trigger;}
    void setStop(QString trigger) {m_stopTrigger = trigger;}
    void setFile(QString file);
    void setRepeat(bool repeat) {m_repeat = repeat;}
};

#endif // AUDIOCOMPONENT_H
