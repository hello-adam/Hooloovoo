#include "graphicsscene.h"
#include "gameobject.h"
#include "gamecore.h"
#include <QMenu>
#include <QPainter>
#include <QtOpenGL/qgl.h>
#include <QGraphicsItem>
#include <QGraphicsView>
#include "physicsmanager.h"
#include "gamefiledialog.h"
#include "leveldatadialog.h"
#include "filemanager.h"

GraphicsScene::GraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{
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
    Q_UNUSED(rect)
    if (m_isPaused)
    {
        painter->setPen(Qt::green);
        painter->setBrush(Qt::black);
        painter->setFont(QFont("Consolas", 24));
        painter->drawText(
                    QRectF(this->views().at(0)->mapToScene(this->views().at(0)->width()/2 - 80, this->views().at(0)->height()/2 - 12), QSizeF(160, 25)),
                    "PAUSED", QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));

        //draw border of world
        painter->setPen(Qt::red);
        painter->setBrush(Qt::transparent);
        painter->drawRect(PhysicsManager::getInstance().getBoundingRect());
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
    QAction *editGlobal = menu.addAction("Edit Level Properties");
    QAction *selectedAction = menu.exec(event->screenPos());

    QWidget *parent = GameCore::getInstance().getDialogParent();
    if (selectedAction == addObject)
    {
        GameFileDialog dlg(parent);

        dlg.setAcceptMode(GameFileDialog::Select);
        dlg.setFileType(GameFileDialog::GameObject);

        if (dlg.exec() && !dlg.getFileName().isEmpty())
        {
            GameCore::getInstance().addObjectToCurrentLevel(FileManager::getInstance().loadGameObject(dlg.getFileName()), event->scenePos());
        }
    }
    else if (selectedAction == editGlobal)
    {
        LevelDataDialog dlg(parent);
        dlg.editLevelData();
    }

    delete addObject;
    delete editGlobal;
}

void GraphicsScene::pause()
{
    m_isPaused = true;
    this->update();
}

void GraphicsScene::unpause()
{
    m_isPaused = false;
    this->update();
}
