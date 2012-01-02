#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QResizeEvent>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsView(QWidget *parent = 0);

protected:
    QSize m_resolution;

    void resizeEvent(QResizeEvent *event);

signals:

public slots:
    void setResolution(QSize resolution);

};

#endif // GRAPHICSVIEW_H
