#include "graphicsview.h"
#include "gamecore.h"

GraphicsView::GraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
    m_resolution = QSize(0, 0);
}

void GraphicsView::setResolution(QSize resolution)
{
    m_resolution = resolution;
    this->resetTransform();
    this->scale(double(this->width())/double(resolution.width()), double(this->height())/double(resolution.height()));
}

void GraphicsView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);

    if (m_resolution.width() > 0)
        setResolution(m_resolution);
}

void GraphicsView::focusOutEvent(QFocusEvent *event)
{
    emit lostFocus();

    QGraphicsView::focusOutEvent(event);
}
