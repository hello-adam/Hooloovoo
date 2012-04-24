#include "audiocomponent.h"
#include "filemanager.h"

AudioComponent::AudioComponent(GameObject *parentObject) :
    Component(parentObject)
{
    this->setObjectName("Audio Component");
    m_tag = "Audio";

    m_file = "";
    m_repeat = false;

    m_audioObject = new Phonon::MediaObject(this);
    m_audioOutput = new Phonon::AudioOutput(Phonon::GameCategory, this);

    m_path = Phonon::createPath(m_audioObject, m_audioOutput);

    m_properties << new Property(this, "audioFile", Property::AudioDialog);
    m_properties << new Property(this, "volume");
    m_properties << new Property(this, "repeat");
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
    properties << "audioFile" << "repeat" << "volume" << "tag";
    return properties;
}

void AudioComponent::setRepeat(bool repeat)
{
    m_repeat = repeat;
    if (repeat)
    {
        connect(m_audioObject, SIGNAL(finished()),
                this, SLOT(forceRepeat()));
    }
    else
    {
        disconnect(m_audioObject, SIGNAL(finished()),
                this, SLOT(forceRepeat()));
    }
}

void AudioComponent::effectPlay()
{
    m_audioObject->play();
}

void AudioComponent::effectStop()
{
    m_audioObject->stop();
}

void AudioComponent::effectPause()
{
    m_audioObject->pause();
}

void AudioComponent::forceRepeat()
{
    m_audioObject->stop();
    m_audioObject->play();
}

void AudioComponent::setFile(QString file)
{
    m_file = file;

    m_audioObject->setCurrentSource(FileManager::getInstance().getAudioPath() + file);
}
