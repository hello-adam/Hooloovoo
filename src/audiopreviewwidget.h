#ifndef AUDIOPREVIEWWIDGET_H
#define AUDIOPREVIEWWIDGET_H

#include "filepreviewwidget.h"
#include "phonon"

namespace Ui {
    class AudioPreviewWidget;
}

class AudioPreviewWidget : public FilePreviewWidget
{
    Q_OBJECT

public:
    explicit AudioPreviewWidget(QWidget *parent = 0);
    ~AudioPreviewWidget();

private:
    Ui::AudioPreviewWidget *ui;
    Phonon::MediaObject *m_audioObject;
    Phonon::AudioOutput *m_audioOutput;

public slots:
    void setFile(QString fileName);
};

#endif // AUDIOPREVIEWWIDGET_H
