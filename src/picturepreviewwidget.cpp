#include "picturepreviewwidget.h"
#include "ui_picturepreviewwidget.h"

PicturePreviewWidget::PicturePreviewWidget(QWidget *parent) :
    FilePreviewWidget(parent),
    ui(new Ui::PicturePreviewWidget)
{
    ui->setupUi(this);

    ui->lb_pic->
}

PicturePreviewWidget::~PicturePreviewWidget()
{
    delete ui;
}

void PicturePreviewWidget::setFile(QString fileName)
{
    ui->lb_pic->setPixmap(QPixmap(FileManager::getInstance().getPicturePath() + fileName));
}
