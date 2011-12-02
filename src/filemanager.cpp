#include "filemanager.h"
#include "gameobject.h"
#include <QApplication>

FileManager::FileManager(QObject *parent) :
    QObject(parent)
{
    m_activeGameFileDirectory = "";
    m_activeGameFileName = "";

    QDir dir;
    if (!QDir(getDefaultGamesPath()).exists())
        dir.mkdir(getDefaultGamesPath());
}

QDomElement FileManager::createNewGame(const QString &gameDirectoryName, const QString &startLevel)
{
    QDir gameDir(gameDirectoryName);
    if (gameDir.exists())
        return QDomElement();

    m_activeGameFileDirectory = getDefaultGamesPath() + gameDirectoryName;
    m_activeGameFileName = gameDirectoryName + ".game";

    QDir dir;
    dir.mkdir(getActiveGamePath());

    dir.mkdir(getPicturePath());
    dir.mkdir(getAudioPath());
    dir.mkdir(getObjectPath());
    dir.mkdir(getLevelPath());
    dir.mkdir(getPlayStatePath());

    saveGame(startLevel);

    GameObject* baseObject = new GameObject();
    baseObject->saveObject("baseobject.gameobject");
    delete baseObject;

    return loadGame(gameDirectoryName);
}

QDomElement FileManager::loadGame(const QString &fileDirectory)
{
    QDomDocument doc;

    QString fullDirectory = getDefaultGamesPath() + fileDirectory;

    QDir gamedir(fullDirectory);

    if (gamedir.entryList(getGameExtensions(), QDir::Files).isEmpty())
        return QDomElement();

    QString fileName = gamedir.entryList(getGameExtensions(), QDir::Files).at(0);

    m_activeGameFileDirectory = fullDirectory;
    m_activeGameFileName = fileName;

    QFile file(getActiveGamePath() + fileName);

    file.open(QFile::ReadOnly);
    doc.setContent(&file);
    file.close();

    QDomElement element = doc.documentElement();

    QDomElement level = element.firstChildElement("startlevel");
    QString startLevel = level.attribute("file");

    return loadLevel(startLevel);
}

bool FileManager::saveGame(QString startLevel, bool overwrite)
{
    QDomDocument doc;
    QDomElement data = doc.createElement("game");

    QStringList levels = getAvailableLevels();

    if (levels.isEmpty() && startLevel.isEmpty())
    {
        createNewLevel("firstLevel.level");
        startLevel = "firstLevel.level";
    }
    else if (!startLevel.isEmpty() && !levels.contains(startLevel))
    {
        createNewLevel(startLevel);
    }
    else if (startLevel.isEmpty() && !levels.contains(startLevel))
    {
        startLevel = levels.at(0);
    }

    QDomElement levelElem = doc.createElement("startlevel");
    levelElem.setAttribute("file", startLevel);
    data.appendChild(levelElem);

    QFile file(getActiveGamePath() + m_activeGameFileName);
    file.open(QFile::WriteOnly | QFile::Truncate);
    QTextStream fileStream(&file);
    data.save(fileStream, 2);
    file.close();

    return true;
}

QDomElement FileManager::createNewLevel(const QString &fileName)
{
    QDomDocument doc;
    QDomElement data = doc.createElement("level");

    QDomElement levelData = doc.createElement("leveldata");
    data.appendChild(levelData);

    QFile file(getLevelPath() + fileName);

    file.open(QFile::WriteOnly | QFile::Truncate);
    QTextStream fileStream(&file);
    data.save(fileStream, 2);
    file.close();

    return loadLevel(fileName);
}

QDomElement FileManager::loadLevel(const QString &fileName)
{
    QFile file(getLevelPath() + fileName);
    if (!file.exists())
        return QDomElement();

    QDomDocument doc;
    file.open(QFile::ReadOnly);
    doc.setContent(&file);
    file.close();

    QDomElement element = doc.documentElement();

    return element;
}

bool FileManager::saveLevel(const QDomElement &data, const QString &fileName, bool overwrite)
{
    QFile file(getLevelPath() + fileName);

    if (file.exists() && !overwrite)
        return false;

    file.open(QFile::WriteOnly | QFile::Truncate);
    QTextStream fileStream(&file);
    data.save(fileStream, 2);
    file.close();

    return true;
}

QDomElement FileManager::loadGameObject(const QString &fileName)
{
    QDomDocument doc;

    QFile file(getObjectPath() + fileName);
    if (!file.exists())
        return QDomElement();

    file.open(QFile::ReadOnly);
    doc.setContent(&file);
    file.close();

    QDomElement element = doc.documentElement();

    return element;
}

bool FileManager::saveGameObject(const QDomElement &data, const QString &fileName, bool overwrite)
{
    QFile file(getObjectPath() + fileName);

    if (file.exists() && !overwrite)
        return false;

    file.open(QFile::WriteOnly | QFile::Truncate);
    QTextStream fileStream(&file);
    data.save(fileStream, 2);
    file.close();

    return true;
}

QDomElement FileManager::loadPlayState(const QString &fileName)
{
    QDomDocument doc;

    QFile file(getPlayStatePath() + fileName);
    if (!file.exists())
        return QDomElement();

    file.open(QFile::ReadOnly);
    doc.setContent(&file);
    file.close();

    QDomElement element = doc.documentElement();

    return element;
}

bool FileManager::savePlayState(const QDomElement &data, const QString &fileName, bool overwrite)
{
    QFile file(getObjectPath() + fileName);

    if (file.exists() && !overwrite)
        return false;

    file.open(QFile::WriteOnly | QFile::Truncate);
    QTextStream fileStream(&file);
    data.save(fileStream, 2);
    file.close();

    return true;
}

QStringList FileManager::getAvailableGameObjects()
{
    QDir dir(getObjectPath());

    return dir.entryList(getObjectExtensions(), QDir::Files);
}

QStringList FileManager::getAvailableLevels()
{
    QDir dir(getLevelPath());

    return dir.entryList(getLevelExtensions(), QDir::Files);
}

QStringList FileManager::getAvailableGames()
{
    QDir dir(getDefaultGamesPath());
    QStringList files = dir.entryList(QStringList("*"), QDir::Dirs | QDir::NoDotAndDotDot);

    QStringList knownGameDirectories;
    foreach (QString file, files)
    {
        QStringList contents = QDir(getDefaultGamesPath() + file).entryList(getGameExtensions(), QDir::Files);
        if (!contents.isEmpty())
            knownGameDirectories << file;
    }

    return knownGameDirectories;
}

QStringList FileManager::getAvailablePlayStates()
{
    QDir dir(getPlayStatePath());

    return dir.entryList(getPlayStateExtensions(), QDir::Files);
}

QStringList FileManager::getAvailablePictures()
{
    QDir dir(getPicturePath());

    return dir.entryList(getPictureExtensions(), QDir::Files);
}

QStringList FileManager::getAvailableAudio()
{
    QDir dir(getAudioPath());

    return dir.entryList(getAudioExtensions(), QDir::Files);
}

QString FileManager::getDefaultGamesPath()
{
    return QApplication::applicationDirPath() + "/games/";
}

QString FileManager::getActiveGamePath()
{
    return m_activeGameFileDirectory + "/";
}

QString FileManager::getPicturePath()
{
    return getActiveGamePath() + "/pictures/";
}

QString FileManager::getLevelPath()
{
    return getActiveGamePath() + "/levels/";
}

QString FileManager::getPlayStatePath()
{
    return getActiveGamePath() + "/playstates/";
}

QString FileManager::getObjectPath()
{
    return getActiveGamePath() + "/objects/";
}

QString FileManager::getAudioPath()
{
    return getActiveGamePath() + "/audio/";
}
