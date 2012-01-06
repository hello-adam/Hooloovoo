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
#include <QRgb>

GraphicsScene::GraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{
    connect(&PhysicsManager::getInstance(), SIGNAL(boundsChanged()),
            this, SLOT(update()));
    m_showResolutionRect = false;

    m_backgroundColor = QColor::fromRgbF(0.3f, 0.7f, 0.8f);
}

void GraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    if (painter->paintEngine()->type()!= QPaintEngine::OpenGL)
    {
        painter->setBrush(m_backgroundColor);
        painter->drawRect(rect);
    }

    glClearColor(m_backgroundColor.redF(),
                 m_backgroundColor.greenF(),
                 m_backgroundColor.blueF(),
                 m_backgroundColor.alphaF());
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

        if (m_showResolutionRect)
        {
            //draw border of in-game sight
            painter->setPen(Qt::DashLine);
            QRectF visibleRegion = QRectF(QPointF(0, 0),
                                          GameCore::getInstance().getResolution());
            visibleRegion.moveCenter(this->views().at(0)->mapToScene(this->views().at(0)->width()/2, this->views().at(0)->height()/2));
            painter->drawRect(visibleRegion);
        }
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
        dlg.setFileType(FileManager::Object);

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

void GraphicsScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    const QMimeData *data = event->mimeData();

    if (!data->hasFormat("application/vnd.text.list"))
        QGraphicsScene::dragMoveEvent(event);
    else
        event->accept();
}

void GraphicsScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    const QMimeData *data = event->mimeData();

    if (!data->hasFormat("application/vnd.text.list"))
        QGraphicsScene::dropEvent(event);

    QByteArray textData = data->data("application/vnd.text.list");
    QDataStream textStream(&textData, QIODevice::ReadOnly);
    QStringList textList;

    while (!textStream.atEnd())
    {
        QString text;
        textStream >> text;
        textList << text;
    }

    foreach (QString fileName, textList)
    {
        if (fileName.endsWith(FileManager::getObjectExtensions().at(0).mid(1)))
        {
            GameCore::getInstance().addObjectToCurrentLevel(FileManager::getInstance().loadGameObject(fileName), event->scenePos());
        }
    }

    event->accept();
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
