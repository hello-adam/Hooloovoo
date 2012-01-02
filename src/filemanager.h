#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QtXml>

class FileManager : public QObject
{
    Q_OBJECT
public:
    static FileManager &getInstance()
    {
        static FileManager instance;

        return instance;
    }

    enum FileType { Picture, Audio, Object, Level, Game, PlayState };

    bool createNewGame(const QString &gameDirectoryName, const QString &startLevel = QString());
    QDomElement loadGame(const QString &fileDirectory);
    bool saveGame(QString startLevel = QString(), QSize resolution = QSize(800, 600), bool overwrite = true);

    QDomElement createNewLevel(const QString &fileName);
    QDomElement loadLevel(const QString &fileName);
    bool saveLevel(const QDomElement &data, const QString &fileName, bool overwrite = true);
    bool removeLevel(const QString &fileName);

    QDomElement loadGameObject(const QString &fileName);
    bool saveGameObject(const QDomElement &data, const QString &fileName, bool overwrite = true);

    QDomElement loadPlayState(const QString &fileName);
    bool savePlayState(const QDomElement &data, const QString &fileName, bool overwrite = true);

    QStringList getAvailableFilesOfType(FileType type);
    static QStringList getFileExtensionsForType(FileType type);
    QString getActivePathForType(FileType type);

    QStringList getAvailableGameObjects();
    QStringList getAvailableLevels();
    QStringList getAvailableGames();
    QStringList getAvailablePlayStates();
    QStringList getAvailablePictures();
    QStringList getAvailableAudio();

    static QStringList getObjectExtensions() {return QStringList("*.gameobject");}
    static QStringList getLevelExtensions() {return QStringList("*.level");}
    static QStringList getGameExtensions() {return QStringList("*.game");}
    static QStringList getPlayStateExtensions() {return QStringList("*.playstate");}
    static QStringList getPictureExtensions() {return QStringList("*.png");}
    static QStringList getAudioExtensions() {return QStringList("*.wav");}


    static QString getDefaultGamesPath();
    QString getActiveGamePath();
    QString getLevelPath();
    QString getObjectPath();
    QString getPlayStatePath();
    QString getPicturePath();
    QString getAudioPath();

private:
    explicit FileManager(QObject *parent = 0);

    QString m_activeGameFileDirectory;
    QString m_activeGameFileName;

signals:
    void gameObjectAddedOrRemoved();

public slots:

};

#endif // FILEMANAGER_H
