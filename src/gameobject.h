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

    enum MouseEditMode { Nothing, Move, ResizeBottomRight, RotateHorizontal, RotateVertical };

    void setPixmapFile(QString fileName);
    void setTemporaryPixmapFile(QString fileName);
    QString getPixmapFile() {return m_pixmapFileName;}
    void setVisibleInGame(bool visibleInGame) {m_visibleInGame = visibleInGame;}
    bool getVisibleInGame() {return m_visibleInGame;}
    void setTag(QString tag) {m_tag = tag;}
    QString getTag() {return m_tag;}
    void setDefaultColor(QColor color) {m_defaultColor = color; setPixmapFile(m_pixmapFileName);}
    QColor getDefaultColor() {return m_defaultColor;}
    QList<QPolygonF> getTessellation() { if (m_tessellation.isEmpty()) createTesselation(); return m_tessellation;}

    void paint (QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
    QRectF boundingRect() const;

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    void setPaused(bool pause);

    QSet<QString> getEditProperties();

    double width() {return m_pixmap.width()*scale();}
    double height() {return m_pixmap.height()*scale();}

    Component* addComponent(QString name);
    bool removeComponent(Component* component);
    QList<Component*> getComponents() {return m_components;}

    QDomElement serialize();
    bool deserialize(const QDomElement &objectElement);

    bool polarLessThan(const QPoint &a, const QPoint &b);


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
    QList<QPolygonF> m_tessellation;

    MouseEditMode m_modeOnClick;
    MouseEditMode m_currentMode;
    QPointF m_startPoint;
    void resizeToRect(QRectF rect);
    QRectF m_outlineRect;

    void createTesselation();
    QVector<QPoint> grahamScan(QList<QPoint> points, int minYIndex);

signals:
    void sendX(double x);
    void sendY(double y);
    void sendRotation(double degrees);
    void sendLocalTrigger(QString);
    void sendGlobalTrigger(QString);
    void componentAdded(Component*);
    void componentRemoved(Component*);
    void newTessellation();

private slots:
    void slotXChanged() {emit sendX(x());}
    void slotYChanged() {emit sendY(y());}
    void slotRotationChanged() {emit sendRotation(rotation());}
    void checkLocalTrigger(QString trigger);

public slots:
    void launchEditorDialog();
    void saveObject(QString fileName);
    void launchSaveDialog();
    void emitLocalTrigger(QString trigger) {if (!trigger.isEmpty()) emit sendLocalTrigger(trigger);}
};

#endif // GAMEOBJECT_H
