#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
class GameObject;

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphicsScene(QObject *parent = 0);

    void pause();
    void unpause();

    void setBackgroundColor(QColor color) {m_backgroundColor = color;}
    QColor getBackgroundColor() {return m_backgroundColor;}

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);
    void drawForeground(QPainter *painter, const QRectF &rect);

    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
//    void keyPressEvent(QKeyEvent *event);
//    void keyReleaseEvent(QKeyEvent *event);

    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);

private:
    QList<GameObject*> m_gameObjects;
    bool m_isPaused;
    bool m_showResolutionRect;
    QColor m_backgroundColor;

signals:

public slots:
    void setShowResolutionRect(bool show) {m_showResolutionRect = show; this->update();}

};

#endif // GRAPHICSSCENE_H
