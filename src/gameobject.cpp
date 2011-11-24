#include "gameobject.h"
#include "gamecore.h"
#include "gameobjecteditdialog.h"
#include <QPainter>
#include <QDebug>
#include <QVariant>
#include <QMenu>
#include <QGraphicsSceneMouseEvent>
#include "physicscomponent.h"
#include "physicsmanager.h"
#include "componentfactory.h"
#include "graphicsscene.h"
#include "gamefiledialog.h"

GameObject::GameObject(QGraphicsItem * parent) :
    QGraphicsObject(parent)
{
    setObjectName("Game Object");


    m_visibleInGame = true;
    m_tag = "";
    m_defaultColor = QColor(0, 0, 128);
    this->setPixmapFile("");

    m_paused = false;

    this->setFlag(QGraphicsItem::ItemIsFocusable, true);
    this->setPaused(true);

    this->setAcceptHoverEvents(true);
    this->setAcceptedMouseButtons(Qt::RightButton | Qt::LeftButton);

    connect(this, SIGNAL(sendLocalTrigger(QString)),
            this, SLOT(checkLocalTrigger(QString)));
}

GameObject::~GameObject()
{
    //remove it from its scene if it has one
    if (this->scene())
    {
        GraphicsScene* scene = qobject_cast<GraphicsScene*>(this->scene());

        if (scene)
        {
            scene->removeGameObject(this);
        }
        else
        {
            this->scene()->removeItem(this);
        }
    }

    //remove its components
    foreach (Component* component, m_components)
    {
        delete component;
    }
    m_components.clear();
}

void GameObject::setPixmapFile(QString fileName)
{
    m_pixmapFileName = fileName;
    if (!m_pixmap.load(GameCore::getPicturePath() + fileName))
    {
        m_pixmap = QPixmap(42, 42);
        m_pixmap.fill(m_defaultColor);
        m_pixmapFileName = "";
    }
}

void GameObject::setTemporaryPixmapFile(QString fileName)
{
    if (!m_pixmap.load(GameCore::getPicturePath() + fileName))
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
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->setOpacity(this->opacity());
    painter->drawPixmap(boundingRect().toRect(), m_pixmap);

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

void GameObject::setPaused(bool pause)
{
    if (pause)
    {
        m_paused = true;
        this->setFlag(QGraphicsItem::ItemIsMovable, true);

        if (!m_visibleInGame)
            this->setVisible(true);

        connect(this, SIGNAL(xChanged()),
                this, SLOT(slotXChanged()));
        connect(this, SIGNAL(yChanged()),
                this, SLOT(slotYChanged()));
        connect(this, SIGNAL(rotationChanged()),
                this, SLOT(slotRotationChanged()));
    }
    else
    {
        m_paused = false;
        this->setFlag(QGraphicsItem::ItemIsMovable, false);

        if (!m_visibleInGame)
            this->setVisible(false);

        disconnect(this, SIGNAL(xChanged()),
                this, SLOT(slotXChanged()));
        disconnect(this, SIGNAL(yChanged()),
                this, SLOT(slotYChanged()));
        disconnect(this, SIGNAL(rotationChanged()),
                this, SLOT(slotRotationChanged()));
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
        m_components.push_back(component);
        emit componentAdded(component);
        return component;
    }

    return 0;
}

bool GameObject::removeComponent(Component *component)
{
    if (m_components.contains(component))
    {
        m_components.removeAll(component);
        emit componentRemoved(component);
        delete component;
        return true;
    }

    return false;
}

QDomElement GameObject::serialize(QDomDocument *document)
{
    QDomElement objectElement = document->createElement("gameobject");

    for (int i = 0; i<this->metaObject()->propertyCount(); i++)
    {
        QMetaProperty property = this->metaObject()->property(i);
        QString name = property.name();

        if (property.isValid() && property.type() != QVariant::Invalid && property.userType())
        {
            QVariant value = property.read(this);

            if (property.isEnumType())
            {
                QDomElement prop = document->createElement(name);
                prop.setAttribute("type", value.type());
                QVariant intValue = *reinterpret_cast<const int *>(value.constData());
                prop.setAttribute("value", intValue.toString());
                objectElement.appendChild(prop);
            }
            else if (value.type() == QVariant::StringList)
            {
                QDomElement prop = document->createElement(name);

                QStringList stringList = value.toStringList();
                foreach(QString s, stringList)
                {
                    QDomElement child = document->createElement("listitem");
                    child.setAttribute("value", s);
                    prop.appendChild(child);
                }

                prop.setAttribute("type", value.type());
                prop.setAttribute("count", stringList.count());
                objectElement.appendChild(prop);
            }
            else
            {
                QDomElement prop = document->createElement(name);
                prop.setAttribute("type", value.type());
                prop.setAttribute("value", value.toString());
                objectElement.appendChild(prop);
            }
        }
    }
    //serialize components
    foreach(Component* c, m_components)
    {
        objectElement.appendChild(c->serialize(document));
    }

    return objectElement;
}

bool GameObject::deserialize(const QDomElement &objectElement)
{
    for (int i = 0; i<this->metaObject()->propertyCount(); i++)
    {
        QMetaProperty property = this->metaObject()->property(i);
        QString name = property.name();
        QDomElement prop = objectElement.firstChildElement(name);

        if (prop.hasAttribute("type") && prop.hasAttribute("value"))
        {
            QVariant value;
            value.setValue(prop.attribute("value"));

            if (property.isEnumType())
            {
                value.convert(QVariant::Int);
                property.write(this, value);
            }
            else if (value.convert((QVariant::Type)prop.attribute("type").toInt()))
            {
                property.write(this, value);
            }
        }
        else if (prop.hasAttribute("type") && prop.hasAttribute("count"))
        {
            QVariant value;
            QStringList stringList;

            QDomElement child = prop.firstChildElement("listitem");
            while (!child.isNull())
            {
                stringList << child.attribute("value");
                child = child.nextSiblingElement("listitem");
            }

            value.setValue(stringList);
            property.write(this, value);
        }
    }

    //deserialize components
    ComponentFactory factory;
    QDomElement component = objectElement.firstChildElement("component");
    while (!component.isNull())
    {
        Component* c = factory.createComponent(this, component);
        if (c)
        {
            m_components.push_back(c);
            emit componentAdded(c);
        }
        component = component.nextSiblingElement("component");
    }

    return true;
}

QSet<QString> GameObject::getEditProperties()
{
    QSet<QString> set;
    set << "x" << "y" << "z" << "opacity" << "pixmapFileName" << "scale" << "rotation" << "visibleInGame" << "tag" <<"defaultColor";
    return set;
}

void GameObject::launchEditorDialog()
{
    GameObjectEditDialog* dlg = new GameObjectEditDialog();
    dlg->editObject(this);
    dlg->deleteLater();
}

void GameObject::saveObject(QString fileName)
{
    QFile *file = new QFile(GameCore::getObjectPath() + fileName);

    file->open(QFile::WriteOnly | QFile::Truncate);

    QTextStream fileStream(file);

    QDomDocument *doc = new QDomDocument();
    QDomElement object = this->serialize(doc);

    doc->appendChild(object);

    doc->save(fileStream, 2);
    delete doc;

    file->close();
    delete file;
}

void GameObject::checkLocalTrigger(QString trigger)
{
    if (trigger.contains(QRegExp("Tag(.*)")))
    {
        int start = trigger.indexOf("Tag(")+4;
        int end = trigger.indexOf(")", start);
        QString tag = trigger.mid(start, end-start);

        trigger.remove("Tag(" + tag + ")");

        if (m_tag != tag)
            return;
    }

    if (trigger.contains(QRegExp("Property(.*)")))
    {
        int start = trigger.indexOf("Property(")+9;
        int end = trigger.indexOf(")", start);
        QString argString = trigger.mid(start, end-start);
        QStringList args = argString.split(',', QString::KeepEmptyParts);

        trigger.remove("Property(" + argString + ")");

        if (args.count() == 3)
        {
            if (args.at(0) == this->objectName())
            {
                if (this->metaObject()->indexOfProperty(args.at(1).toStdString().c_str()) >= 0)
                {
                    this->setProperty(args.at(1).toStdString().c_str(), QVariant(args.at(2)));
                }
            }
        }
    }

    if (trigger == "Destroy")
        this->deleteLater();
}

void GameObject::launchSaveDialog()
{
    GameFileDialog *dlg = new GameFileDialog();
    dlg->setFileExtensions(QStringList("*.gameobject"));
    dlg->setSubdirectory("objects");
    dlg->setAccept("Save");

    if (dlg->exec())
    {
        saveObject(dlg->getFileName());
    }

    delete dlg;
}
