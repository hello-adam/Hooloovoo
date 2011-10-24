#include "graphicsscene.h"
#include "gameobject.h"
#include "gamecore.h"
#include <QMenu>
#include <QPainter>
#include <QtOpenGL/qgl.h>
#include <QGraphicsItem>
#include <QGraphicsView>

GraphicsScene::GraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{
}

void GraphicsScene::addGameObject(GameObject* gameObject)
{
    m_gameObjects.push_back(gameObject);
    this->addItem(gameObject);
    gameObject->setPaused(m_isPaused);
}

void GraphicsScene::removeGameObject(GameObject *gameObject)
{
    m_gameObjects.removeAll(gameObject);
    this->removeItem(gameObject);
}

void GraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    if (painter->paintEngine()->type()!= QPaintEngine::OpenGL)
    {
        painter->setBrush(Qt::blue);
        painter->drawRect(rect);
    }

    glClearColor(0.3f, 0.7f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GraphicsScene::drawForeground(QPainter *painter, const QRectF &rect)
{
    if (this->focusItem())
    {
        painter->setPen(Qt::green);
        painter->setBrush(Qt::transparent);

        painter->drawRect(this->focusItem()->sceneBoundingRect());
    }

    if (m_isPaused)
    {
        painter->setPen(Qt::green);
        painter->setBrush(Qt::black);
        painter->setFont(QFont("Consolas", 24));
        painter->drawText(
                    QRectF(this->views().at(0)->mapToScene(this->views().at(0)->width()/2 - 80, this->views().at(0)->width()/2 - 10), QSizeF(160, 20)),
                    "PAUSED", QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));
    }
}

void GraphicsScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QGraphicsScene::contextMenuEvent(event);

    if (event->isAccepted())
        return;

    event->accept();

    QMenu menu;
    QAction *addObject = menu.addAction("Add Object");
    QAction *editGlobal = menu.addAction("Edit World");
    QAction *selectedAction = menu.exec(event->screenPos());

    if (selectedAction == addObject)
    {
        GameCore::getInstance()->launchAddObjectDialog(event->screenPos());
    }
    else if (selectedAction == editGlobal)
    {
        GameCore::getInstance()->launchGlobalDataDialog();
    }

    delete addObject;
    delete editGlobal;
}

void GraphicsScene::keyPressEvent(QKeyEvent *event)
{
    GameCore::getInstance()->handleKeyEvent(event);
}

void GraphicsScene::keyReleaseEvent(QKeyEvent *event)
{
    GameCore::getInstance()->handleKeyEvent(event);
}

void GraphicsScene::distributeKeyEvent(QKeyEvent *event)
{
    foreach(GameObject* go, m_gameObjects)
    {
        go->distributeKeyEvent(event);
    }
}

void GraphicsScene::clearAll()
{
    qDeleteAll(m_gameObjects);
    m_gameObjects.clear();
    this->clear();
}

void GraphicsScene::pause()
{
    foreach(GameObject* object, m_gameObjects)
    {
        object->setPaused(true);
    }

    m_isPaused = true;
    this->update();
}

void GraphicsScene::unpause()
{
    foreach(GameObject* object, m_gameObjects)
    {
        object->setPaused(false);
    }

    m_isPaused = false;
    this->update();
}
