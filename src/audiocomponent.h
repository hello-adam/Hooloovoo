#ifndef AUDIOCOMPONENT_H
#define AUDIOCOMPONENT_H

#include "component.h"
#include <phonon>

class AudioComponent : public Component
{
    Q_OBJECT
    Q_PROPERTY(bool repeat READ getRepeat WRITE setRepeat)
    Q_PROPERTY(QString audioFile READ getFile WRITE setFile)
    Q_PROPERTY(double volume READ getVolume WRITE setVolume)

public:
    AudioComponent(GameObject *parentObject);
    ~AudioComponent();

    QSet<QString> getEditProperties();

    QString getFile() {return m_file;}
    bool getRepeat() {return m_repeat;}
    double getVolume() {return m_audioOutput->volume()*11.0;}

private:
    QString m_file;
    bool m_repeat;
    Phonon::MediaObject *m_audioObject;
    Phonon::AudioOutput *m_audioOutput;
    Phonon::Path m_path;

    void reactToTrigger(QString trigger);

signals:
    void causePlaybackFinished();

public slots:
    void setFile(QString file);
    void setRepeat(bool repeat);
    void setVolume(double volume) {m_audioOutput->setVolume(volume/11.0);}

    void effectPlay();
    void effectStop();
    void effectPause();

    void forceRepeat();
};

#endif // AUDIOCOMPONENT_H
