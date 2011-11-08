#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QGraphicsObject>
#include <QSharedPointer>
#include <QUuid>
#include <QDialog>
#include "component.h"

class GameObject : public QGraphicsObject
{
    Q_OBJECT
    Q_PROPERTY(QString pixmapFileName READ getPixmapFile WRITE setPixmapFile)
    Q_PROPERTY(QString tag READ getTag WRITE setTag)
    Q_PROPERTY(QColor defaultColor READ getDefaultColor WRITE setDefaultColor)
    Q_PROPERTY(bool visibleInGame READ getVisibleInGame WRITE setVisibleInGame)

public:
    GameObject(QGraphicsItem  *parent = 0);
    ~GameObject();

    void setPixmapFile(QString fileName);
    QString getPixmapFile() {return m_pixmapFileName;}
    void setVisibleInGame(bool visibleInGame) {m_visibleInGame = visibleInGame;}
    bool getVisibleInGame() {return m_visibleInGame;}
    void setTag(QString tag) {m_tag = tag;}
    QString getTag() {return m_tag;}
    void setDefaultColor(QColor color) {m_defaultColor = color; setPixmapFile(m_pixmapFileName);}
    QColor getDefaultColor() {return m_defaultColor;}

    void paint (QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
    QRectF boundingRect() const;

    void setPaused(bool pause);

    QSet<QString> getEditProperties();

    double width() {return m_pixmap.width()*scale();}
    double height() {return m_pixmap.height()*scale();}

    Component* addComponent(QString name);
    bool removeComponent(Component* component);
    QList<Component*> getComponents() {return m_components;}

    QDomElement serialize(QDomDocument *document);
    bool deserialize(const QDomElement &objectElement);

    void distributeKeyEvent(QKeyEvent *event);


protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    QList<Component*> m_components;
    QUuid m_uid;
    QString m_pixmapFileName;
    QPixmap m_pixmap;
    bool m_visibleInGame;
    bool m_paused;
    QString m_tag;
    QColor m_defaultColor;

signals:
    void sendX(double x);
    void sendY(double y);
    void sendRotation(double degrees);
    void sendLocalEvent(QString);
    void sendGlobalEvent(QString);
    void componentAdded(Component*);
    void componentRemoved(Component*);

private slots:
    void slotXChanged() {emit sendX(x());}
    void slotYChanged() {emit sendY(y());}
    void slotRotationChanged() {emit sendRotation(rotation());}
    void checkLocalEvent(QString trigger);

public slots:
    void launchEditorDialog();
    void saveObject(QString fileName);
    void launchSaveDialog();
    void emitLocalEvent(QString trigger) {emit sendLocalEvent(trigger);}
};

#endif // GAMEOBJECT_H
