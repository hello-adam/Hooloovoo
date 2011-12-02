#ifndef FILEPREVIEWWIDGET_H
#define FILEPREVIEWWIDGET_H

#include <QWidget>

class FilePreviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FilePreviewWidget(QWidget *parent = 0);

signals:

public slots:
    virtual void setFile(QString fileName) = 0;
};

#endif // FILEPREVIEWWIDGET_H
