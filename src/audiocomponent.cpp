#include "audiocomponent.h"
#include "gamecore.h"

AudioComponent::AudioComponent(GameObject *parentObject) :
    Component(parentObject)
{
    this->setObjectName("Audio Component");

    m_timeStep = 10;
    m_startTrigger = "";
    m_stopTrigger = "";
    m_file = "";
    m_completeTrigger = "";
    m_repeat = false;

    m_audioObject = new Phonon::MediaObject(this);
    m_audioOutput = new Phonon::AudioOutput(Phonon::GameCategory, this);

    m_path = Phonon::createPath(m_audioObject, m_audioOutput);
}

AudioComponent::~AudioComponent()
{
    if (m_audioObject)
        delete m_audioObject;
    if (m_audioOutput)
        delete m_audioOutput;
}

QSet<QString> AudioComponent::getEditProperties()
{
    QSet<QString> properties;
    properties << "audioFile" << "triggerToStart" << "triggerToStop" << "completeTrigger" << "repeat" << "volume";
    return properties;
}

void AudioComponent::setFile(QString file)
{
    m_file = file;

    m_audioObject->setCurrentSource(GameCore::getAudioPath() + file);
}

void AudioComponent::reactToTrigger(QString trigger)
{
    if (!m_audioObject)
        return;

    if (trigger == m_startTrigger)
    {
        m_audioObject->play();
    }
    else if (trigger == m_stopTrigger)
    {
        m_audioObject->stop();
    }
}
