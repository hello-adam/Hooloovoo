#include "audiopreviewwidget.h"
#include "ui_audiopreviewwidget.h"

AudioPreviewWidget::AudioPreviewWidget(QWidget *parent) :
    FilePreviewWidget(parent),
    ui(new Ui::AudioPreviewWidget)
{
    ui->setupUi(this);

    m_audioObject = new Phonon::MediaObject(this);
    m_audioOutput = new Phonon::AudioOutput(Phonon::GameCategory, this);

    Phonon::createPath(m_audioObject, m_audioOutput);

    connect(ui->pb_play, SIGNAL(clicked()),
            m_audioObject, SLOT(play()));
    connect(ui->pb_pause, SIGNAL(clicked()),
            m_audioObject, SLOT(pause()));
    connect(ui->pb_stop, SIGNAL(clicked()),
            m_audioObject, SLOT(stop()));

    connect(m_audioObject, SIGNAL(finished()),
            m_audioObject, SLOT(stop()));
}

AudioPreviewWidget::~AudioPreviewWidget()
{
    delete ui;
}

void AudioPreviewWidget::setFile(QString fileName)
{
    m_audioObject->stop();
    m_audioObject->setCurrentSource(FileManager::getInstance().getAudioPath() + fileName);
}
