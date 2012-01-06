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
#include "componentalteration.h"
#include "levelmanager.h"

class GameCore : public QObject
{
    Q_OBJECT
    Q_ENUMS(Command)
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
    GraphicsScene* getGraphicsScene() {return m_scene;}
    void setDialogParent(QWidget* parent) {m_dialogParent = parent;}
    QWidget* getDialogParent() {return m_dialogParent;}

    bool createGame(QString gameName);
    bool loadGame(QString gameName);
    bool saveCurrentGame();
    bool setCurrentGameStartLevel(QString levelName);
    QString getCurrentGameStartLevel() {return m_currentGameStartLevel;}
    QString getCurrentLevel() {return m_currentLevelName;}

    int addObjectToCurrentLevel(const QDomElement &objectElement, QPointF pos = QPointF());
    bool removeObjectFromCurrentLevel(int objectID);
    bool alterComponent(int objectID, const ComponentAlteration &alteration);
    void destroyDeadObjects();

    void handleKeyEvent(QKeyEvent *ke);
    void addInputReceiver(InputReceiver* receiver) {m_inputReceivers.push_back(receiver);}
    void removeInputReceiver(InputReceiver* receiver) {m_inputReceivers.removeAll(receiver);}

    void issueCommand(Command command, QString parameter);
    void dealWithCommands();

    bool isPaused() {return m_isPaused;}

    GameObject* selectedObject();

    QAction* getTogglePauseAction() {return m_togglePaused;}
    QAction* getToggleResolutionVisibleAction() {return m_toggleResolutionVisible;}
    QAction* getSavePlayStateAction() {return m_savePlayState;}
    QAction* getLoadPlayStateAction() {return m_loadPlayState;}
    QAction* getSwitchGameAction() {return m_switchGame;}
    QAction* getCreateGameAction() {return m_createGame;}
    QAction* getSaveLevelAction() {return m_saveLevel;}
    QAction* getEditCurrentLevelAction() {return m_editCurrentLevel;}
    QAction* getAddObjectToLevelAction() {return m_addObjectToLevel;}
    QAction* getEditSelectedObjectAction() {return m_editSelectedObject;}
    QAction* getSaveSelectedObjectAction() {return m_saveSelectedObject;}
    QAction* getRemoveSelectedObjectAction() {return m_removeSelectedObject;}
    QAction* getCopySelectedObjectAction() {return m_copySelectedObject;}
    QAction* getPasteSelectedObjectAction() {return m_pasteSelectedObject;}

    void setResolution(int width, int height) {m_resolution = QSize(width, height); emit resolutionChanged();}
    QSize getResolution() {return m_resolution;}

    bool getObjectSatisfiesTag(int id, QString tag);

private:
    explicit GameCore(QObject *parent = 0);
    static GameCore *m_instance;
    QTimer m_gameTimer;

    QHash<int, GameObject*> m_gameObjectsByID;
    QSet<GameObject*> m_gameObjects;
    int getAvailableGameObjectID();

    int m_centeredObjectID;

    QList<InputReceiver*> m_inputReceivers;

    QString m_currentGameName;
    QString m_currentLevelName;
    QString m_currentGameStartLevel;

    bool m_isPaused;
    void pause();
    void unpause();

    QDomElement m_clipBoardElement;
    QString m_newLevel;

    GraphicsScene* m_scene;
    QWidget* m_dialogParent;

    QDomElement serializeSelectedObject();
    int m_selectedObjectID;

    void destroyLevel();
    QDomElement serializeLevel();
    bool deserializeLevel(const QDomElement &levelElement);

    QAction* m_toggleResolutionVisible;
    QAction* m_togglePaused;
    QAction* m_savePlayState;
    QAction* m_loadPlayState;
    QAction* m_switchGame;
    QAction* m_createGame;
    QAction* m_saveLevel;
    QAction* m_editCurrentLevel;
    QAction* m_addObjectToLevel;
    QAction* m_editSelectedObject;
    QAction* m_saveSelectedObject;
    QAction* m_removeSelectedObject;
    QAction* m_copySelectedObject;
    QAction* m_pasteSelectedObject;

    QSize m_resolution;

signals:
    void timerTick();

    void hasSelectedObject(bool);
    void hasObjectOnClipboard(bool);

    void gamesChanged();
    void levelDataChanged();
    void resolutionChanged();

public slots:
    void gameStep();

    void togglePaused();
    void togglePaused(bool pause);

    void setCenteredObject(int objectLevelID) {m_centeredObjectID = objectLevelID;}

    void savePlayState();
    void loadPlayState();

    void switchGame();
    void createGame();

    void saveCurrentLevel();
    void launchEditLevelDialog();

    void addObjectToCurrentLevelSlot();
    void editSelectedObject();
    void saveSelectedObject();
    void removeSelectedObject();

    void copySelectedObjectToClipboard();
    void pasteClipboardObjectToCurrentLevel(QPointF pos = QPointF());

    void checkSceneSelection();

    void adjustSceneSize();
};

#endif // CORE_H
