#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QGraphicsItem>
#include <QSharedPointer>
#include <QUuid>
#include <QDialog>
#include "component.h"
class CauseEffectManager;

class GameObject : public Component, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
    Q_PROPERTY(bool visibleInGame READ getVisibleInGame WRITE setVisibleInGame)
    Q_PROPERTY(QString pixmapFileName READ getPixmapFile WRITE setPixmapFile)
    Q_PROPERTY(QPointF position READ pos WRITE setPos)
    Q_PROPERTY(int zCoordinate READ zValue WRITE setZValue)
    Q_PROPERTY(double opacityAmount READ opacity WRITE setOpacity)
    Q_PROPERTY(double clockwiseRotation READ rotation WRITE setRotation)
    Q_PROPERTY(QColor defaultColor READ getDefaultColor WRITE setDefaultColor)
    Q_PROPERTY(double scaleFactor READ scale WRITE setScale)

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
    QList<Component*> getComponents() {return m_IDsByComponents.keys();}
    Component* getComponentByID (int ID) {return m_componentRegistry.value(ID, 0);}

    bool polarLessThan(const QPoint &a, const QPoint &b);

    int getAvailableComponentID();

    CauseEffectManager* getCauseEffectManager();

    int getComponentID(Component* component) {return m_IDsByComponents.value(component, -1);}

    void initiateObject() {emit causeInitiated();}

    bool isDestroyed() {return m_destroyed;}

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    QUuid m_uid;
    QString m_pixmapFileName;
    QPixmap m_pixmap;
    bool m_visibleInGame;
    bool m_paused;
    QString m_tag;
    QColor m_defaultColor;
    QList<QPolygonF> m_tessellation;

    bool m_destroyed;

    QHash<int, Component*> m_componentRegistry;
    QHash<Component*, int> m_IDsByComponents;
    QSet<int> m_registeredIDs;

    MouseEditMode m_modeOnClick;
    MouseEditMode m_currentMode;
    QPointF m_startPoint;
    void resizeToRect(QRectF rect);
    QRectF m_outlineRect;

    void createTesselation();
    QVector<QPoint> grahamScan(QList<QPoint> points, int minYIndex);

    CauseEffectManager* m_causeEffectManager;

    void privateSerialize(QDomElement &componentObject);
    void privateDeserialize(const QDomElement &componentObject);

signals:
    void sendX(double x);
    void sendY(double y);
    void sendRotation(double degrees);

    void componentAdded(Component*);
    void componentRemoved(Component*);
    void newTessellation();

    void causeInitiated();

private slots:
    void slotXChanged() {emit sendX(x());}
    void slotYChanged() {emit sendY(y());}
    void slotRotationChanged() {emit sendRotation(rotation());}

public slots:
    void launchEditorDialog();
    void saveObject(QString fileName);
    void launchSaveDialog();
    void effectDestroy() {m_destroyed = true;}
};

#endif // GAMEOBJECT_H
