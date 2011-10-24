#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QTimer>

#include "graphicsscene.h"
#include "gameobject.h"
#include "componentfactory.h"
#include "physicsmanager.h"

class GameCore : public QObject
{
    Q_OBJECT
public:
    static GameCore* getInstance();
    ~GameCore();

    void setScene(GraphicsScene* scene) {m_scene = scene; if (m_isPaused) m_scene->pause(); else m_scene->unpause();}
    void addObjectToScene(QString fileName, QPointF pos);

    void launchGlobalDataDialog();
    void launchAddObjectDialog(QPoint pos = QPoint());

    void saveGame(QString fileName);
    void loadGame(QString fileName);

    QDomElement serializeGlobal(QDomDocument *document);
    bool deserializeGlobal(const QDomElement &objectElement);

    QString getPicturePath();
    QString getGamePath();
    QString getObjectPath();

    void handleKeypress(QKeyEvent *ke);

private:
    GraphicsScene* m_scene;
    explicit GameCore(QObject *parent = 0);
    static GameCore *m_instance;

    bool m_isPaused;
    void pause();
    void unpause();

signals:
    void triggerRepaint();

public slots:
    void togglePaused();
};

#endif // CORE_H
