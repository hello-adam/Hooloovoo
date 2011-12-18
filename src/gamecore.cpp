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
    connect(m_scene, SIGNAL(selectionChanged()),
            this, SLOT(checkSceneSelection()));

    m_dialogParent = 0;

    m_centeredObjectID = -1;

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

//    m_manageLevels = new QAction("&Manage Levels", this);
//    connect(m_manageLevels, SIGNAL(triggered()), this, SLOT(launchManageLevelsDialog()));

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

    m_removeSelectedObject = new QAction("&Remove Selected Object", this);
    connect(m_removeSelectedObject, SIGNAL(triggered()), this, SLOT(copySelectedObjectToClipboard()));
    connect(this, SIGNAL(hasSelectedObject(bool)), m_removeSelectedObject, SLOT(setEnabled(bool)));

    m_copySelectedObject = new QAction("&Copy Selected Object", this);
    connect(m_copySelectedObject, SIGNAL(triggered()), this, SLOT(copySelectedObjectToClipboard()));
    connect(this, SIGNAL(hasSelectedObject(bool)), m_copySelectedObject, SLOT(setEnabled(bool)));

    m_pasteSelectedObject = new QAction("&Paste Object", this);
    connect(m_pasteSelectedObject, SIGNAL(triggered()), this, SLOT(pasteClipboardObjectToCurrentLevel()));
    connect(this, SIGNAL(hasObjectOnClipboard(bool)), m_pasteSelectedObject, SLOT(setEnabled(bool)));

    emit hasSelectedObject(false);
    emit hasObjectOnClipboard(false);
}

GameCore::~GameCore()
{
}

void GameCore::gameStep()
{
    if (m_gameObjectsByID.value(m_centeredObjectID, 0))
    {
        GameObject* centeredObject = m_gameObjectsByID.value(m_centeredObjectID);

        for (int i=0; i<m_scene->views().count(); i++)
        {
            m_scene->views().at(i)->centerOn(centeredObject);
        }
    }

    PhysicsManager::getInstance().takeStep();
    emit timerTick();
    this->destroyDeadObjects();

    dealWithCommands();
}

void GameCore::dealWithCommands()
{
    if (!m_newLevel.isEmpty())
    {
        QDomElement levelElem;
        levelElem = FileManager::getInstance().loadLevel(m_newLevel);

        if (!levelElem.isNull())
        {
            deserializeLevel(levelElem);
            m_currentLevelName = m_newLevel;

            emit levelDataChanged();
        }

        m_newLevel = "";
    }
}

void GameCore::issueCommand(Command command, QString parameter)
{
    if (command == ChangeLevel)
    {
        if (!parameter.endsWith(FileManager::getLevelExtensions().at(0).mid(1)))
            parameter += FileManager::getLevelExtensions().at(0).mid(1);
        m_newLevel = parameter;
    }

    if (isPaused())
        dealWithCommands();
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

    GameObject *gameObject = new GameObject(ID);
    gameObject->setPaused(true);
    gameObject->deserialize(objectElement);
    if (!pos.isNull())
    {
        gameObject->setPos(pos);
    }
    gameObject->initiateObject();
    gameObject->setPaused(m_isPaused);

    m_gameObjects.insert(gameObject);
    m_gameObjectsByID.insert(ID, gameObject);
    m_scene->addItem(gameObject);

    return ID;
}

bool GameCore::removeObjectFromCurrentLevel(int objectID)
{
    GameObject* object = m_gameObjectsByID.value(objectID, 0);

    if (!object)
        return false;

    object->effectDestroy();
    return true;
}

bool GameCore::alterComponent(int objectID, const ComponentAlteration &alteration)
{
    return true;
}

void GameCore::destroyDeadObjects()
{
    QList<int> ids = m_gameObjectsByID.keys();

    foreach (int id, ids)
    {
        GameObject *object = m_gameObjectsByID.value(id);

        if (!object)
        {
            m_gameObjectsByID.remove(id);
            m_gameObjects.remove(0);
        }
        else if (object->isDestroyed())
        {
            m_gameObjectsByID.remove(id);
            m_gameObjects.remove(object);
            m_scene->removeItem(object);

            delete object;
        }
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

    if (this->deserializeLevel(FileManager::getInstance().loadLevel(startLevel)))
    {
        m_currentGameStartLevel = startLevel;
        m_currentLevelName = startLevel;
        m_currentGameName = gameName;
        emit levelDataChanged();
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

void GameCore::destroyLevel()
{
    foreach (GameObject *object, m_gameObjects)
    {
        object->effectDestroy();
    }

    destroyDeadObjects();

    m_gameObjectsByID.clear();
    m_gameObjects.clear();

    m_scene->clear();
    m_scene->update();
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

    foreach(GameObject* g, m_gameObjects)
    {
        level.appendChild(g->serialize());
    }

    return level;
}

bool GameCore::deserializeLevel(const QDomElement &levelElement)
{
    bool unpaused = !m_isPaused;

    this->pause();
    destroyLevel();

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
        if (gameobject.attribute("name") == "Game Object")
        {
            this->addObjectToCurrentLevel(gameobject);
        }

        gameobject = gameobject.nextSiblingElement("component");
    }


    emit levelDataChanged();
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
    if (m_scene)
        m_scene->pause();
    foreach(GameObject *object, m_gameObjects)
    {
        object->setPaused(true);
    }
    m_isPaused = true;
}

void GameCore::unpause()
{
    if (m_scene)
        m_scene->unpause();
    foreach(GameObject *object, m_gameObjects)
    {
        object->setPaused(false);
    }

    m_isPaused = false;
    m_gameTimer.start();
}

void GameCore::togglePaused()
{
    togglePaused(!m_isPaused);
}

void GameCore::togglePaused(bool pause)
{
    if (pause)
        this->pause();
    else
        this->unpause();
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

void GameCore::launchEditLevelDialog()
{
    LevelDataDialog dlg(m_dialogParent);
    dlg.editLevelData();
}

void GameCore::addObjectToCurrentLevelSlot()
{
    GameFileDialog dlg(m_dialogParent);

    dlg.setAcceptMode(GameFileDialog::Load);
    dlg.setFileType(GameFileDialog::GameObject);

    if (dlg.exec())
    {
        if (!dlg.getFileName().isEmpty())
        {
            QPointF centerScreen = QPointF(42, 42);
            if (m_scene->views().count() > 0)
            {
                centerScreen = m_scene->views().at(0)->mapToScene(m_scene->views().at(0)->width()/2, m_scene->views().at(0)->height()/2);
            }

            addObjectToCurrentLevel(FileManager::getInstance().loadGameObject(dlg.getFileName()), centerScreen);
        }
    }
}

void GameCore::editSelectedObject()
{
    if (!m_gameObjectsByID.value(m_selectedObjectID, 0))
        return;

    m_gameObjectsByID.value(m_selectedObjectID)->launchEditorDialog();
}

void GameCore::saveSelectedObject()
{
    if (!m_gameObjectsByID.value(m_selectedObjectID, 0))
        return;

    m_gameObjectsByID.value(m_selectedObjectID)->launchSaveDialog();
}

void GameCore::removeSelectedObject()
{
    removeObjectFromCurrentLevel(m_selectedObjectID);
}

void GameCore::copySelectedObjectToClipboard()
{
    m_clipBoardElement = serializeSelectedObject();

    emit hasObjectOnClipboard(!m_clipBoardElement.isNull());
}

void GameCore::pasteClipboardObjectToCurrentLevel(QPointF pos)
{
    if (!m_clipBoardElement.isNull())
    {
        if (pos.isNull())
        {
            pos = QPointF(42, 42);
            if (m_scene->views().count() > 0)
            {
                pos = m_scene->views().at(0)->mapToScene(m_scene->views().at(0)->width()/2, m_scene->views().at(0)->height()/2);
            }
        }

        addObjectToCurrentLevel(m_clipBoardElement, pos);
    }
}

QDomElement GameCore::serializeSelectedObject()
{
    QDomElement selected;

    if (m_gameObjectsByID.value(m_selectedObjectID, 0))
    {
        selected = m_gameObjectsByID.value(m_selectedObjectID)->serialize();
    }

    return selected;
}

void GameCore::checkSceneSelection()
{
    if (m_scene->selectedItems().count() > 0)
    {
        if (dynamic_cast<GameObject*>(m_scene->selectedItems().at(0)))
        {
            GameObject *object = dynamic_cast<GameObject*>(m_scene->selectedItems().at(0));
            m_selectedObjectID = m_gameObjectsByID.key(object, -1);

            if (m_selectedObjectID >= 0)
            {
                emit hasSelectedObject(true);
                return;
            }
        }
    }

    emit hasSelectedObject(false);
}
