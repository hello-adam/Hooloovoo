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
#include <QMenuBar>

class GameCore : public QObject
{
    Q_OBJECT
public:
    enum Command
    {
        ChangeLevel
    };


    static GameCore &getInstance()
    {
        static GameCore instance;

        return instance;
    }
    ~GameCore();

    QDomElement getClipboardElement() {return m_clipBoardElement;}

    QDomElement serializeSelectedObject();
    void addObjectToLevel(const QDomElement &objectElement, QPointF pos = QPoint());

    QDomElement serializeLevel();
    bool deserializeLevel(const QDomElement &levelElement);

    void handleKeyEvent(QKeyEvent *ke);

    void addInputReceiver(InputReceiver* receiver) {m_inputReceivers.push_back(receiver);}
    void removeInputReceiver(InputReceiver* receiver) {m_inputReceivers.removeAll(receiver);}

    GraphicsScene* getGraphicsScene() {return m_scene;}

private:
    explicit GameCore(QObject *parent = 0);
    static GameCore *m_instance;
    QTimer m_gameTimer;

    QList<InputReceiver*> m_inputReceivers;

    bool m_isPaused;
    void pause();
    void unpause();

    QDomElement m_clipBoardElement;

    GraphicsScene* m_scene;

signals:
    void timerTick();

    void hasSelectedObject(bool);
    void hasObjectOnClipboard(bool);

public slots:
    void togglePaused();

    void copySelectedObjectToClipboard();
};

#endif // CORE_H
