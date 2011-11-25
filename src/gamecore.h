#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QTimer>

#include "graphicsscene.h"
#include "gameobject.h"
#include "componentfactory.h"
#include "physicsmanager.h"
#include "inputreceiver.h"
#include <phonon>

class GameCore : public QObject
{
    Q_OBJECT
public:
    static GameCore* getInstance();
    ~GameCore();

    void initializeGameDirectoriesAndData();

    void setScene(GraphicsScene* scene);
    void addObjectToScene(QString fileName, QPointF pos);

    void launchGlobalDataDialog();
    void launchAddObjectDialog(QPoint pos = QPoint());

    void saveGame(QString fileName);
    void loadGame(QString fileName);

    QDomElement serializeGlobal(QDomDocument *document);
    bool deserializeGlobal(const QDomElement &objectElement);

    static QString getPicturePath();
    static QString getGamePath();
    static QString getObjectPath();
    static QString getAudioPath();

    void handleKeyEvent(QKeyEvent *ke);

    void addInputReceiver(InputReceiver* receiver) {m_inputReceivers.push_back(receiver);}
    void removeInputReceiver(InputReceiver* receiver) {m_inputReceivers.removeAll(receiver);}

    Phonon::AudioOutput* getAudioOutput() {return m_audioOutput;}

private:
    GraphicsScene* m_scene;
    explicit GameCore(QObject *parent = 0);
    static GameCore *m_instance;
    QTimer m_gameTimer;

    QList<InputReceiver*> m_inputReceivers;

    bool m_isPaused;
    void pause();
    void unpause();

    Phonon::AudioOutput *m_audioOutput;

signals:
    void timerTick();

public slots:
    void togglePaused();
};

#endif // CORE_H
