#ifndef PICTUREPREVIEWWIDGET_H
#define PICTUREPREVIEWWIDGET_H

#include "filepreviewwidget.h"

namespace Ui {
    class PicturePreviewWidget;
}

class PicturePreviewWidget : public FilePreviewWidget
{
    Q_OBJECT

public:
    explicit PicturePreviewWidget(QWidget *parent = 0);
    ~PicturePreviewWidget();

private:
    Ui::PicturePreviewWidget *ui;

public slots:
    void setFile(QString fileName);
};

#endif // PICTUREPREVIEWWIDGET_H
