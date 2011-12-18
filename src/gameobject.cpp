#include "gameobject.h"
#include "gamecore.h"
#include "gameobjecteditdialog.h"
#include <QPainter>
#include <QDebug>
#include <QVariant>
#include <QMenu>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include "physicscomponent.h"
#include "physicsmanager.h"
#include "componentfactory.h"
#include "graphicsscene.h"
#include "gamefiledialog.h"
#include "filemanager.h"
#include "causeeffectmanager.h"

GameObject::GameObject(int levelID) :
    Component(0),
    QGraphicsItem(0)
{
    setObjectName("Game Object");

    m_ID = 0;
    m_levelID = levelID;
    m_destroyed = false;

    m_causeEffectManager = new CauseEffectManager(this);

    m_visibleInGame = true;
    m_tag = "";
    m_defaultColor = QColor(0, 0, 128);
    this->setPixmapFile("");

    m_modeOnClick = Nothing;
    m_currentMode = Nothing;
    m_startPoint = QPointF();

    m_paused = true;

    this->setFlag(QGraphicsItem::ItemIsSelectable, true);

    this->setAcceptHoverEvents(true);
    this->setAcceptedMouseButtons(Qt::RightButton | Qt::LeftButton);
}

GameObject::~GameObject()
{
    //remove its components
    qDeleteAll(m_components);
    m_componentRegistry.clear();

    delete m_causeEffectManager;
}

void GameObject::initiateObject()
{
    emit causeInitiated();
}

CauseEffectManager* GameObject::getCauseEffectManager()
{
    return m_causeEffectManager;
}

int GameObject::getAvailableComponentID()
{
    QSet<int> ids = m_componentRegistry.keys().toSet();
    for (int i = 1; i<5000; i++)
    {
        if (!ids.contains(i))
        {
            return i;
        }
    }

    return -1;
}

void GameObject::setPixmapFile(QString fileName)
{
    m_pixmapFileName = fileName;

    m_tessellation.clear();

    if (!m_pixmap.load(FileManager::getInstance().getPicturePath() + fileName))
    {
        m_pixmap = QPixmap(42, 42);
        m_pixmap.fill(m_defaultColor);
        m_pixmapFileName = "";
    }

    createTesselation();
}

void GameObject::setTemporaryPixmapFile(QString fileName)
{
    if (!m_pixmap.load(FileManager::getInstance().getPicturePath() + fileName))
    {
        m_pixmap = QPixmap(42, 42);
        m_pixmap.fill(m_defaultColor);
    }
}

void GameObject::createTesselation()
{
    if (m_pixmap.isNull())
        return;

    QList<QPoint> outlinePoints;
    QImage image = m_pixmap.toImage();

    for(int y=0; y<image.height(); y++)
    {
        for (int x=0; x<image.width(); x++)
        {
            if (qAlpha(image.pixel(x, y)) > 0)
            {
                if (x>0 && x+1<image.width() && y>0 && y+1<image.height())
                {
                    if (qAlpha(image.pixel(x-1, y)) == 0
                            || qAlpha(image.pixel(x+1, y)) == 0
                            || qAlpha(image.pixel(x, y-1)) == 0
                            || qAlpha(image.pixel(x+1, y+1)) == 0)
                    {
                        outlinePoints.push_back(QPoint(x, y));
                    }
                }
                else
                {
                    outlinePoints.push_back(QPoint(x, y));
                }
            }
        }
    }

    QVector<QPoint> original = grahamScan(outlinePoints, 0);
    QVector<QPoint> reversed;
    for (int i=original.count()-1; i>=0; i--)
    {
        reversed.push_back(original.at(i));
    }

    //Split up the bounding polygon into smaller polygons if there are too many points (this currently only works for convex polygons)
    QPolygonF polygonVector(reversed);
    int count = polygonVector.count();
    QList<QPolygonF> tessellation;
    if (count > 8)
    {
        int leftOver = 0;
        int mainSize = 2;
        int tesselations = 2;
        while (leftOver < 3)
        {
            mainSize++;
            leftOver = count%mainSize;
            if (leftOver == 0)
            {
                leftOver = mainSize;
                tesselations = count/mainSize;
            }
            else
            {
                tesselations = count/mainSize + 1;
            }
        }

        QPointF center = QPointF(m_pixmap.width()/2, m_pixmap.height()/2);

        for (int i=0; i<tesselations-1; i++)
        {
            QPolygonF polygon;
            for (int j=0; j<=mainSize; j++)
            {
                polygon.push_back(polygonVector.at(i*mainSize+j));
            }
            polygon.push_back(center);
            tessellation.push_back(polygon);
        }
        QPolygonF lastPart;
        for (int j=0; j<leftOver; j++)
        {
            lastPart.push_back(polygonVector.at((tesselations-1)*mainSize+j));
        }
        lastPart.push_back(polygonVector.at(0));
        lastPart.push_back(center);
        tessellation.push_back(lastPart);
    }
    else
    {
        tessellation.push_back(polygonVector);
    }

    m_tessellation = tessellation;
    emit newTessellation();
}

QVector<QPoint> GameObject::grahamScan(QList<QPoint> points, int minYIndex)
{
    if (minYIndex >= points.count() || minYIndex < 0)
        return QVector<QPoint>();

    QPoint minY = points.takeAt(minYIndex);

    //sort by polar angle around minY
    QMap<double, QPoint> pointsByCosine;
    foreach(QPoint point, points)
    {
        QPoint diff = point - minY;
        double cosine = -1.0*diff.x() / sqrt(diff.x()*diff.x() + diff.y()*diff.y());
        if (pointsByCosine.value(cosine, QPoint()).isNull())
        {
            pointsByCosine.insert(cosine, point);
        }
        else
        {
            QPoint otherDiff = pointsByCosine.value(cosine, QPoint()) - minY;
            if (otherDiff.manhattanLength() < diff.manhattanLength())
            {
                pointsByCosine.remove(cosine);
                pointsByCosine.insert(cosine, point);
            }
        }
    }
    points = pointsByCosine.values();

    QVector<QPoint> hull;
    hull << minY;
    hull << points.takeFirst();

    while (!points.isEmpty())
    {
        QPoint next = points.takeFirst();
        QPoint top = hull.at(hull.count()-1);
        QPoint oneFromTop = hull.at(hull.count()-2);

        while ((top.x()-oneFromTop.x())*(next.y()-oneFromTop.y()) - (top.y()-oneFromTop.y())*(next.x()-oneFromTop.x()) <= 0)
        {
               hull.pop_back();

               if (hull.count() < 2)
                   break;

               top = hull.at(hull.count()-1);
               oneFromTop = hull.at(hull.count()-2);
        }
        hull.push_back(next);
    }

    return hull;
}

void GameObject::paint (QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->setOpacity(this->opacity());
    painter->drawPixmap(boundingRect().toRect(), m_pixmap);

    if (this->isSelected() && m_paused)
    {
        painter->setPen(Qt::green);
        if (m_outlineRect.isNull())
        {
            painter->drawRect(this->boundingRect());
            painter->drawRect(QRectF(this->boundingRect().bottomRight() - QPointF(7, 7), QSizeF(7, 7)));
        }
        else
        {
            painter->drawRect(m_outlineRect);
        }
    }


    //Enable this code in order to see the object's tesselation
//    QList<QPolygonF> tessellation = getTessellation();
//    painter->setPen(Qt::red);
//    foreach (QPolygonF poly, tessellation)
//    {
//        painter->drawPolygon(poly.translated(m_pixmap.width()/-2, m_pixmap.height()/-2));
//    }
}

QRectF GameObject::boundingRect() const
{
    return QRectF(m_pixmap.width()/-2.0, m_pixmap.height()/-2.0,
                  m_pixmap.width(), m_pixmap.height());
}

void GameObject::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    event->accept();

    QMenu menu;
    QAction *editAction = menu.addAction("Edit Object");
    QAction *removeAction = menu.addAction("Remove Object");
    QAction *saveAction = menu.addAction("Save Object");
    QAction *selectedAction = menu.exec(event->screenPos());

    if (selectedAction == editAction)
    {
        launchEditorDialog();
    }
    else if (selectedAction == removeAction)
    {
        this->deleteLater();
    }
    else if (selectedAction == saveAction)
    {
        launchSaveDialog();
    }

    delete editAction;
    delete removeAction;
    delete saveAction;
}

void GameObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);

    if (event->button() != Qt::LeftButton || !m_paused)
    {
        event->ignore();
        return;
    }

    m_currentMode = m_modeOnClick;
    m_startPoint = event->scenePos();

    if (m_currentMode == ResizeBottomRight)
    {
        m_outlineRect = boundingRect();
    }
}

void GameObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!m_paused)
    {
        event->ignore();
        return;
    }

    if (m_currentMode == Move)
    {
        QPointF diff = event->scenePos()-m_startPoint;
        this->setPos(this->pos()+diff);
        m_startPoint = event->scenePos();
    }
    else if (m_currentMode == ResizeBottomRight)
    {
        m_outlineRect = boundingRect();
        QPointF diff = event->scenePos() - m_startPoint;
        if (diff.x()/this->width() > diff.y()/this->height())
            diff.setX(diff.y()*this->width()/this->height());
        else
            diff.setY(diff.x()*this->height()/this->width());

        m_outlineRect.setBottomRight(m_outlineRect.bottomRight() + diff/scale());
        this->update();
    }
    else if (m_currentMode == RotateHorizontal)
    {
        QPointF diff = event->scenePos()-m_startPoint;
        this->setRotation(this->rotation()+diff.x());
        m_startPoint = event->scenePos();
    }
    else if (m_currentMode == RotateVertical)
    {
        QPointF diff = event->scenePos()-m_startPoint;
        this->setRotation(this->rotation()+diff.y());
        m_startPoint = event->scenePos();
    }
}

void GameObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);

    if (event->button() != Qt::LeftButton || !m_paused)
    {
        event->ignore();
        return;
    }

    if (m_currentMode == Move)
    {

    }
    else if (m_currentMode == ResizeBottomRight)
    {
        this->resizeToRect(m_outlineRect);
        m_outlineRect = QRectF();
        this->update();
    }
    else if (m_currentMode == RotateHorizontal)
    {

    }

    m_currentMode = Nothing;
}

void GameObject::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (!m_paused)
    {
        event->ignore();
        return;
    }

    bool leftBorder = false;
    bool rightBorder = false;
    bool bottomBorder = false;
    bool topBorder = false;


    if (event->pos().x() < this->boundingRect().width()/-2+7)
    {
        leftBorder = true;
    }
    if (event->pos().x() > this->boundingRect().width()/2-7)
    {
        rightBorder = true;
    }
    if (event->pos().y() < this->boundingRect().height()/-2+7)
    {
        topBorder = true;
    }
    if (event->pos().y() > this->boundingRect().height()/2-7)
    {
        bottomBorder = true;
    }

    if (bottomBorder && rightBorder)
    {
        setCursor(Qt::SizeFDiagCursor);
        m_modeOnClick = ResizeBottomRight;
    }
    else if (bottomBorder || topBorder)
    {
        setCursor(Qt::SizeHorCursor);
        m_modeOnClick = RotateHorizontal;
    }
    else if (rightBorder || leftBorder)
    {
        setCursor(Qt::SizeVerCursor);
        m_modeOnClick = RotateVertical;
    }
    else
    {
        setCursor(Qt::SizeAllCursor);
        m_modeOnClick = Move;
    }
}

void GameObject::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if (!m_paused)
    {
        event->ignore();
        return;
    }

    bool leftBorder = false;
    bool rightBorder = false;
    bool bottomBorder = false;
    bool topBorder = false;


    if (event->pos().x() < this->boundingRect().width()/-2+7)
    {
        leftBorder = true;
    }
    if (event->pos().x() > this->boundingRect().width()/2-7)
    {
        rightBorder = true;
    }
    if (event->pos().y() < this->boundingRect().height()/-2+7)
    {
        topBorder = true;
    }
    if (event->pos().y() > this->boundingRect().height()/2-7)
    {
        bottomBorder = true;
    }

    if (bottomBorder && rightBorder)
    {
        setCursor(Qt::SizeFDiagCursor);
        m_modeOnClick = ResizeBottomRight;
    }
    else if (bottomBorder || topBorder)
    {
        setCursor(Qt::SizeHorCursor);
        m_modeOnClick = RotateHorizontal;
    }
    else if (rightBorder || leftBorder)
    {
        setCursor(Qt::SizeVerCursor);
        m_modeOnClick = RotateVertical;
    }
    else
    {
        setCursor(Qt::SizeAllCursor);
        m_modeOnClick = Move;
    }
}

void GameObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (!m_paused)
    {
        event->ignore();
        return;
    }

    setCursor(Qt::ArrowCursor);
    m_modeOnClick = Nothing;
}

void GameObject::resizeToRect(QRectF rect)
{
    QPointF centerDiff;
    centerDiff = this->mapToScene(rect.center());
    this->setPos(centerDiff);

    double scaleDiff;
    scaleDiff = rect.width()/boundingRect().width();
    this->setScale(scale()*scaleDiff);

    createTesselation();
}

void GameObject::setPaused(bool pause)
{
    if (pause)
    {
        m_paused = true;

        this->setVisible(true);
    }
    else
    {
        m_paused = false;

        if (!m_visibleInGame)
            this->setVisible(false);

        //send any changes to the physics component (or other components that are inter-locked with these values)
        slotXChanged();
        slotYChanged();
        slotRotationChanged();
    }
}

Component* GameObject::addComponent(QString name)
{
    foreach(Component* c, m_components)
    {
        if (c->objectName() == name)
        {
            if (!c->allowMultipleComponents())
                return 0;
        }
    }

    ComponentFactory factory;
    Component* component = factory.createComponent(this, name);
    if (component)
    {
        m_componentRegistry.insert(component->getID(), component);
        m_components.insert(component);

        emit componentAdded(component);
        return component;
    }

    return 0;
}

bool GameObject::removeComponent(Component *component)
{
    if (m_components.contains(component))
    {
        m_componentRegistry.remove(component->getID());
        m_components.remove(component);

        emit componentRemoved(component);
        delete component;
        return true;
    }

    return false;
}

void GameObject::privateSerialize(QDomElement &componentObject)
{
    //serialize components
    foreach(Component* c, m_components)
    {
        componentObject.appendChild(c->serialize());
    }

    //serialize cause effect info
    componentObject.appendChild(m_causeEffectManager->serialize());
}

void GameObject::privateDeserialize(const QDomElement &componentObject)
{
    //deserialize components
    ComponentFactory factory;
    QDomElement component = componentObject.firstChildElement("component");
    while (!component.isNull())
    {
        Component* c = factory.createComponent(this, component);
        if (c)
        {
            m_componentRegistry.insert(c->getID(), c);
            m_components.insert(c);
            emit componentAdded(c);
        }
        component = component.nextSiblingElement("component");
    }

    //deserialize cause and effect
    QDomElement causeEffect = componentObject.firstChildElement("causeeffectdefinitions");
    m_causeEffectManager->deserialize(causeEffect);
}

QSet<QString> GameObject::getEditProperties()
{
    QSet<QString> set;
    set << "position" << "zCoordinate" << "opacityAmount" << "pixmapFileName" << "clockwiseRotation" << "visibleInGame" << "tag" <<"defaultColor";
    return set;
}

void GameObject::launchEditorDialog()
{
    QWidget *parent = GameCore::getInstance().getDialogParent();

    GameObjectEditDialog* dlg = new GameObjectEditDialog(parent);
    dlg->editObject(this);
    dlg->deleteLater();
}

void GameObject::launchSaveDialog()
{
    QWidget *parent = GameCore::getInstance().getDialogParent();

    GameFileDialog *dlg = new GameFileDialog(parent);
    dlg->setAcceptMode(GameFileDialog::Save);
    dlg->setFileType(GameFileDialog::GameObject);

    if (dlg->exec())
    {
        saveObject(dlg->getFileName());
    }

    delete dlg;
}

void GameObject::saveObject(QString fileName)
{
    FileManager::getInstance().saveGameObject(this->serialize(), fileName);
}

void GameObject::effectTakeCameraFocus()
{
    GameCore::getInstance().setCenteredObject(this->getLevelID());
}
