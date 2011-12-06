#include "gamecore.h"
#include <QApplication>
#include <QDebug>
#include <Box2D/Box2D.h>
#include <QKeyEvent>
#include "gamefiledialog.h"
#include <QDialog>
#include <QFormLayout>
#include <QSpinBox>
#include <QDialogButtonBox>
#include "graphicsview.h"
#include <QMenu>
#include "leveldatadialog.h"

GameCore::GameCore(QObject *parent) :
    QObject(parent)
{
    m_scene = new GraphicsScene();
    m_scene->setSceneRect(0, 0, 800, 600);

    qRegisterMetaType<Command>("Command");

    m_gameTimer.setInterval(1000.0 / 60.0);

    connect(&m_gameTimer, SIGNAL(timeout()), this, SLOT(gameStep()));

    emit hasSelectedObject(false);
    emit hasObjectOnClipboard(false);

    this->pause();
}

GameCore::~GameCore()
{
    //qDeleteAll(m_inputReceivers);
}

void GameCore::gameStep()
{
    PhysicsManager::getInstance().takeStep();
    emit timerTick();
    m_scene->destroyDeadObjects();

    if (!m_newLevel.isEmpty())
    {
        QDomElement levelElem;
        levelElem = FileManager::getInstance().loadLevel(m_newLevel);

        if (!levelElem.isNull())
        {
            deserializeLevel(levelElem);
        }

        m_newLevel = "";
    }

}

void GameCore::issueCommand(Command command, QString parameter)
{
    if (command == ChangeLevel)
    {
        m_newLevel = parameter;
    }
}

QDomElement GameCore::serializeSelectedObject()
{
    QDomElement selected;

    if (m_scene->focusItem())
    {

    }

    return selected;
}

void GameCore::copySelectedObjectToClipboard()
{
    m_clipBoardElement = serializeSelectedObject();
}

void GameCore::addObjectToLevel(const QDomElement &objectElement, QPointF pos)
{
    GameObject *gameObject = new GameObject();
    gameObject->deserialize(objectElement);
    gameObject->setPos(pos);
    gameObject->initiateObject();

    if (m_scene)
    {
        m_scene->addGameObject(gameObject);
        m_scene->setFocusItem(gameObject);
    }
    else
    {
        qDebug() << "No scene!  Can't add object";
        delete gameObject;
    }
}

QDomElement GameCore::serializeLevel()
{
    QDomDocument doc;
    QDomElement level = doc.createElement("level");

    QDomElement levelData = doc.createElement("leveldata");

    QDomElement bounds = doc.createElement("bounds");
    bounds.setAttribute("width", PhysicsManager::getInstance().getBoundingRect().width());
    bounds.setAttribute("height", PhysicsManager::getInstance().getBoundingRect().height());
    QDomElement gravity = doc.createElement("gravity");
    gravity.setAttribute("yGravity", PhysicsManager::getInstance().getGravity());

    levelData.appendChild(bounds);
    levelData.appendChild(gravity);

    level.appendChild(levelData);

    QList<GameObject*> gameObjects = m_scene->getGameObjects();
    foreach(GameObject* g, gameObjects)
    {
        level.appendChild(g->serialize());
    }

    return level;
}

bool GameCore::deserializeLevel(const QDomElement &levelElement)
{
    bool unpaused = !m_isPaused;

    this->pause();
    m_scene->clearAll();

    QDomElement dataElement = levelElement.firstChildElement("leveldata");
    QDomElement scene = dataElement.firstChildElement("bounds");
    if (!scene.isNull())
    {
        PhysicsManager::getInstance().setWorldBounds(QRectF(0, 0,
                                                             scene.attribute("width").toDouble(),
                                                             scene.attribute("height").toDouble()));
    }
    QDomElement gravity = dataElement.firstChildElement("gravity");
    if (!gravity.isNull())
    {
        PhysicsManager::getInstance().setGravity(gravity.attribute("yGravity").toDouble());
    }

    QDomElement gameobject = levelElement.firstChildElement("component");

    while (!gameobject.isNull())
    {
        qDebug() << gameobject.attribute("name");

        if (gameobject.attribute("name") == "Game Object")
        {
            GameObject *g = new GameObject();
            g->deserialize(gameobject);

            m_scene->addGameObject(g);
        }

        gameobject = gameobject.nextSiblingElement("component");
    }

    if (unpaused)
        this->unpause();

    return true;
}

void GameCore::togglePaused()
{
    if (m_isPaused)
    {
        unpause();
    }
    else
    {
        pause();
    }
}

void GameCore::pause()
{
    m_gameTimer.stop();
    PhysicsManager::getInstance().pause();
    if (m_scene)
        m_scene->pause();
    m_isPaused = true;
}

void GameCore::unpause()
{
    m_gameTimer.start();
    PhysicsManager::getInstance().start();
    if (m_scene)
        m_scene->unpause();
    m_isPaused = false;
}

void GameCore::handleKeyEvent(QKeyEvent *ke)
{
    if (ke->isAutoRepeat())
    {
        ke->ignore();
        return;
    }

    else
    {
        if (ke->key() == Qt::Key_Space && ke->type() == QKeyEvent::KeyPress)
        {
            this->togglePaused();
        }
        else
        {
            foreach(InputReceiver* receiver, m_inputReceivers)
            {
                receiver->handleKeyEvent(ke);
            }
        }

        ke->accept();
    }
}
