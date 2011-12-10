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

    m_dialogParent = 0;

    qRegisterMetaType<Command>("Command");

    m_gameTimer.setInterval(1000.0 / 60.0);
    connect(&m_gameTimer, SIGNAL(timeout()), this, SLOT(gameStep()));

    m_togglePaused = new QAction("&Pause Game", this);
    m_togglePaused->setCheckable(true);
    connect(m_togglePaused, SIGNAL(toggled(bool)), this, SLOT(togglePaused(bool)));
    m_savePlayState = new QAction("&Save Game", this);
    connect(m_savePlayState, SIGNAL(triggered()), this, SLOT(savePlayState()));
    m_loadPlayState = new QAction("&Load Game", this);
    connect(m_loadPlayState, SIGNAL(triggered()), this, SLOT(loadPlayState()));
    m_switchGame = new QAction("&Switch Game Project", this);
    connect(m_switchGame, SIGNAL(triggered()), this, SLOT(switchGame()));
    m_createGame = new QAction("&Create New Game Project", this);
    connect(m_createGame, SIGNAL(triggered()), this, SLOT(createGame()));
    m_saveLevel = new QAction("&Save Current Level", this);
    connect(m_saveLevel, SIGNAL(triggered()), this, SLOT(saveCurrentLevel()));
    m_manageLevels = new QAction("&Manage Levels", this);
    connect(m_manageLevels, SIGNAL(triggered()), this, SLOT(launchManageLevelsDialog()));
    m_editCurrentLevel = new QAction("&Edit Current Level", this);
    connect(m_editCurrentLevel, SIGNAL(triggered()), this, SLOT(launchEditLevelDialog()));
    m_addObjectToLevel = new QAction("&Add Object to Level", this);
    connect(m_addObjectToLevel, SIGNAL(triggered()), this, SLOT(addObjectToCurrentLevelSlot()));
    m_editSelectedObject = new QAction("&Edit Selected Object", this);
    connect(m_editSelectedObject, SIGNAL(triggered()), this, SLOT(editSelectedObject()));
    connect(this, SIGNAL(hasSelectedObject(bool)), m_editSelectedObject, SLOT(setEnabled(bool)));
    m_saveSelectedObject = new QAction("&Save Selected Object", this);
    connect(m_saveSelectedObject, SIGNAL(triggered()), this, SLOT(saveSelectedObject()));
    connect(this, SIGNAL(hasSelectedObject(bool)), m_saveSelectedObject, SLOT(setEnabled(bool)));
    m_copySelectedObject = new QAction("&Copy Selected Object", this);
    connect(m_copySelectedObject, SIGNAL(triggered()), this, SLOT(copySelectedObjectToClipboard()));
    connect(this, SIGNAL(hasSelectedObject(bool)), m_copySelectedObject, SLOT(setEnabled(bool)));
    m_pasteSelectedObject = new QAction("&Copy Selected Object", this);
    connect(m_pasteSelectedObject, SIGNAL(triggered()), this, SLOT(pasteClipboardObjectToCurrentLevel()));
    connect(this, SIGNAL(hasObjectOnClipboard(bool)), m_pasteSelectedObject, SLOT(setEnabled(bool)));

    emit hasSelectedObject(false);
    emit hasObjectOnClipboard(false);

    this->pause();
}

GameCore::~GameCore()
{
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

int GameCore::getAvailableGameObjectID()
{
    QSet<int> ids = m_gameObjectsByID.keys().toSet();

    for (int i=0; i<42000; i++)
    {
        if (!ids.contains(i))
            return i;
    }

    return -1;
}

int GameCore::addObjectToCurrentLevel(const QDomElement &objectElement, QPointF pos)
{
    int ID = getAvailableGameObjectID();
    if (ID == -1)
    {
        qDebug() << "Too many game objects!  Cannot add more!";
        return -1;
    }

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

bool GameCore::loadGame(QString gameName)
{
    if (!FileManager::getInstance().getAvailableGames().contains(gameName))
    {
        qDebug() << "Game does not exist!";
        return false;
    }

    QDomElement gameElem;
    gameElem = FileManager::getInstance().loadGame(gameName);

    QDomElement level = gameElem.firstChildElement("startlevel");
    QString startLevel = level.attribute("file");

    if (this->deserializeLevel(startLevel))
    {
        m_currentLevelName = startLevel;
        m_currentGameName = gameName;
        return true;
    }
    else
    {
        return false;
    }
}

bool GameCore::createGame(QString gameName)
{
    if (FileManager::getInstance().getAvailableGames().contains(gameName))
    {
        qDebug() << "Game already exists!";
        return false;
    }
    FileManager::getInstance().createNewGame(gameName);

    return loadGame(gameName);
}

bool GameCore::saveCurrentGame()
{
    return FileManager::getInstance().saveGame();
}

bool GameCore::setCurrentGameStartLevel(QString levelName)
{
    m_currentGameStartLevel = levelName;
    return FileManager::getInstance().saveGame(levelName);
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

void GameCore::togglePaused()
{
    togglePaused(!m_isPaused);
}

void GameCore::togglePaused(bool pause)
{
    if (pause)
        pause();
    else
        unpause();
}

void GameCore::savePlayState()
{

}

void GameCore::loadPlayState()
{

}

void GameCore::switchGame()
{
    GameFileDialog dlg(m_dialogParent);

    dlg.setAcceptMode(GameFileDialog::Load);
    dlg.setFileType(GameFileDialog::Game);

    if (dlg.exec() && !dlg.getFileName().isEmpty())
    {
        loadGame(dlg.getFileName());
    }
}

void GameCore::createGame()
{
    GameFileDialog dlg(m_dialogParent);

    dlg.setAcceptMode(GameFileDialog::Create);
    dlg.setFileType(GameFileDialog::Game);

    if (dlg.exec())
    {
        if (!dlg.getFileName().isEmpty())
        {
            createGame(dlg.getFileName());
        }
    }
}

void GameCore::saveCurrentLevel()
{
    FileManager::getInstance().saveLevel(serializeLevel(), m_currentLevelName);
}

void GameCore::launchManageLevelsDialog()
{

}

void GameCore::launchEditLevelDialog()
{
    LevelDataDialog dlg(m_dialogParent);
    dlg.editLevelData();
}

void GameCore::addObjectToCurrentLevelSlot()
{

}

void GameCore::editSelectedObject()
{

}

void GameCore::saveSelectedObject()
{

}

void GameCore::removeSelectedObject()
{

}

void GameCore::copySelectedObjectToClipboard()
{
    m_clipBoardElement = serializeSelectedObject();
}

void GameCore::pasteClipboardObjectToCurrentLevel(QPointF pos = QPointF())
{
    if (!m_clipBoardElement.isNull())
        addObjectToCurrentLevel(m_clipBoardElement);
}

QDomElement GameCore::serializeSelectedObject()
{
    QDomElement selected;

    if (m_scene->focusItem())
    {

    }

    return selected;
}
