#include "graphicsview.h"
#include "gamecore.h"

GraphicsView::GraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
}

void GraphicsView::keyPressEvent(QKeyEvent *ke)
{
    GameCore::getInstance().handleKeyEvent(ke);
}

void GraphicsView::keyReleaseEvent(QKeyEvent *ke)
{
    GameCore::getInstance().handleKeyEvent(ke);
}
