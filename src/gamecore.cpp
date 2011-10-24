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

GameCore* GameCore::m_instance;

GameCore::GameCore(QObject *parent) :
    QObject(parent)
{
    m_scene = 0;

    this->pause();
}

GameCore* GameCore::getInstance()
{
    if (!m_instance)
        m_instance = new GameCore();

    return m_instance;
}

GameCore::~GameCore()
{
}

void GameCore::addObjectToScene(QString fileName, QPointF pos)
{
    QFile *file = new QFile(getObjectPath() + fileName);

    file->open(QFile::ReadOnly);
    QDomDocument *doc = new QDomDocument();
    doc->setContent(file);
    file->close();
    delete file;

    QDomElement object = doc->documentElement();
    delete doc;

    GameObject *gameObject = new GameObject();
    gameObject->deserialize(object);
    gameObject->setPos(pos);

    if (m_scene)
    {
        m_scene->addGameObject(gameObject);
        m_scene->setFocusItem(gameObject);
    }
}

void GameCore::launchGlobalDataDialog()
{
    if (!m_scene)
        return;

    QDialog dlg;
    QFormLayout formLayout(&dlg);
    dlg.setLayout(&formLayout);

    QSpinBox width;
    width.setMaximum(999999999);
    width.setValue(m_scene->width());
    QSpinBox height;
    height.setMaximum(999999999);
    height.setValue(m_scene->height());

    formLayout.addRow("width", &width);
    formLayout.addRow("height", &height);

    QDialogButtonBox buttons(QDialogButtonBox::Ok
                             | QDialogButtonBox::Cancel);
    formLayout.addRow(&buttons);
    connect(&buttons, SIGNAL(accepted()), &dlg, SLOT(accept()));
    connect(&buttons, SIGNAL(rejected()), &dlg, SLOT(reject()));

    if (dlg.exec())
    {
        m_scene->setSceneRect(0, 0, width.value(), height.value());
    }
}

void GameCore::launchAddObjectDialog(QPoint pos)
{
    GameFileDialog *dlg = new GameFileDialog();
    dlg->setFileExtensions(QStringList("*.gameobject"));
    dlg->setSubdirectory("objects");
    dlg->setAccept("Select");

    if (dlg->exec())
    {
        QGraphicsView* view = m_scene->views().at(0);
        if (pos.isNull())
            GameCore::getInstance()->addObjectToScene(dlg->getFileName(), view->mapToScene(view->width()/2.0, view->height()/2.0));
        else
            GameCore::getInstance()->addObjectToScene(dlg->getFileName(), view->mapToScene(view->mapFromGlobal(pos)));
    }

    delete dlg;
}

void GameCore::saveGame(QString fileName)
{
    QFile *file = new QFile(getGamePath() + fileName);

    file->open(QFile::WriteOnly | QFile::Truncate);

    QTextStream fileStream(file);

    QDomDocument *doc = new QDomDocument();
    QDomElement root = doc->createElement("root");

    doc->appendChild(root);

    root.appendChild(serializeGlobal(doc));

    QList<GameObject*> gameObjects = m_scene->getGameObjects();

    foreach(GameObject* g, gameObjects)
    {
        root.appendChild(g->serialize(doc));
    }

    doc->save(fileStream, 2);
    delete doc;

    file->close();
    delete file;
}

void GameCore::loadGame(QString fileName)
{
    if (m_scene)
        m_scene->clearAll();

    QFile *file = new QFile(getGamePath() + fileName);

    file->open(QFile::ReadOnly);
    QDomDocument *doc = new QDomDocument();
    doc->setContent(file);
    file->close();
    delete file;

    QDomElement root = doc->documentElement();
    delete doc;

    QDomElement global = root.firstChildElement("global");
    deserializeGlobal(global);

    QDomElement child = root.firstChildElement("gameobject");

    while (!child.isNull())
    {
        GameObject *g = new GameObject();
        g->deserialize(child);

        if (m_scene)
            m_scene->addGameObject(g);

        child = child.nextSiblingElement("gameobject");
    }
}

void GameCore::handleKeypress(QKeyEvent *ke)
{
    if (ke->key() == Qt::Key_Space)
    {
        this->togglePaused();
    }
    else
    {
        if (m_scene)
            m_scene->distributeKeyPress(ke);
    }
}

void GameCore::pause()
{
    PhysicsManager::getInstance()->pause();
    if (m_scene)
        m_scene->pause();
    m_isPaused = true;
}

void GameCore::unpause()
{
    PhysicsManager::getInstance()->start();
    if (m_scene)
        m_scene->unpause();
    m_isPaused = false;
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

QDomElement GameCore::serializeGlobal(QDomDocument *document)
{
    QDomElement globalElement = document->createElement("global");

    QDomElement scene = document->createElement("scene");
    scene.setAttribute("width", m_scene->width());
    scene.setAttribute("height", m_scene->height());
    globalElement.appendChild(scene);

    return globalElement;
}

bool GameCore::deserializeGlobal(const QDomElement &objectElement)
{
    QDomElement scene = objectElement.firstChildElement("scene");

    if (!scene.isNull())
    {
        if (m_scene)
            m_scene->setSceneRect(0, 0,
                                  scene.attribute("width").toDouble(),
                                  scene.attribute("height").toDouble());
    }

    return true;
}

QString GameCore::getPicturePath()
{
    return QApplication::applicationDirPath() + "/pics/";
}

QString GameCore::getGamePath()
{
    return QApplication::applicationDirPath() + "/games/";
}

QString GameCore::getObjectPath()
{
    return QApplication::applicationDirPath() + "/objects/";
}
