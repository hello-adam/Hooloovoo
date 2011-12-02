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

    QDomElement createNewGame(const QString &gameDirectoryName, const QString &startLevel = QString());
    QDomElement loadGame(const QString &fileDirectory);
    bool saveGame(QString startLevel = QString(), bool overwrite = true);

    QDomElement createNewLevel(const QString &fileName);
    QDomElement loadLevel(const QString &fileName);
    bool saveLevel(const QDomElement &data, const QString &fileName, bool overwrite = true);

    QDomElement loadGameObject(const QString &fileName);
    bool saveGameObject(const QDomElement &data, const QString &fileName, bool overwrite = true);

    QDomElement loadPlayState(const QString &fileName);
    bool savePlayState(const QDomElement &data, const QString &fileName, bool overwrite = true);

    QStringList getAvailableGameObjects();
    QStringList getAvailableLevels();
    QStringList getAvailableGames();
    QStringList getAvailablePlayStates();
    QStringList getAvailablePictures();
    QStringList getAvailableAudio();

    QString getPicturePath();
    QString getAudioPath();

    static QStringList getObjectExtensions() {return QStringList("*.gameobject");}
    static QStringList getLevelExtensions() {return QStringList("*.level");}
    static QStringList getGameExtensions() {return QStringList("*.game");}
    static QStringList getPlayStateExtensions() {return QStringList("*.playstate");}
    static QStringList getPictureExtensions() {return QStringList("*.png");}
    static QStringList getAudioExtensions() {return QStringList("*.wav");}

private:
    explicit FileManager(QObject *parent = 0);

    QString m_activeGameFileDirectory;
    QString m_activeGameFileName;

    static QString getDefaultGamesPath();
    QString getActiveGamePath();
    QString getLevelPath();
    QString getObjectPath();
    QString getPlayStatePath();

signals:

public slots:

};

#endif // FILEMANAGER_H
