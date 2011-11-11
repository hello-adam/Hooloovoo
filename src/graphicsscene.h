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
    QList<GameObject*> getGameObjects() {return m_gameObjects;}
    void addGameObject(GameObject* gameObject);
    void removeGameObject(GameObject* gameObject);

    void pause();
    void unpause();

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);
    void drawForeground(QPainter *painter, const QRectF &rect);

    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    QList<GameObject*> m_gameObjects;
    bool m_isPaused;

signals:

public slots:
    void clearAll();

};

#endif // GRAPHICSSCENE_H
