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

    m_pixmap = QPixmap(42, 42);
    m_pixmap.fill(Qt::darkBlue);
    m_pixmapFileName = "";

    this->setFlag(QGraphicsItem::ItemIsFocusable, true);
    this->setPaused(true);

    this->setAcceptHoverEvents(true);
    this->setAcceptedMouseButtons(Qt::RightButton | Qt::LeftButton);
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
        m_pixmap.fill(Qt::darkBlue);
        m_pixmapFileName = "";
    }
}

void GameObject::paint (QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->setOpacity(this->opacity());
    painter->drawPixmap(boundingRect().toRect(), m_pixmap);
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

void GameObject::distributeKeyEvent(QKeyEvent *event)
{
    if (event->type() == QKeyEvent::KeyPress)
    {
        foreach(Component* c, m_components)
        {
            c->keyPressEvent(event);
        }
    }
    else if (event->type() == QKeyEvent::KeyRelease)
    {
        foreach(Component* c, m_components)
        {
            c->keyReleaseEvent(event);
        }
    }
}

void GameObject::setPaused(bool pause)
{
    if (pause)
    {
        this->setFlag(QGraphicsItem::ItemIsMovable, true);

        connect(this, SIGNAL(xChanged()),
                this, SLOT(slotXChanged()));
        connect(this, SIGNAL(yChanged()),
                this, SLOT(slotYChanged()));
        connect(this, SIGNAL(rotationChanged()),
                this, SLOT(slotRotationChanged()));
    }
    else
    {
        this->setFlag(QGraphicsItem::ItemIsMovable, false);

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

bool GameObject::removeComponent(QString name)
{
    foreach(Component* c, m_components)
    {
        if (c->objectName() == name)
        {
            m_components.removeAll(c);
            emit componentRemoved(c);
            delete c;
            return true;
        }
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

            QDomElement prop = document->createElement(name);
            prop.setAttribute("type", value.type());
            prop.setAttribute("value", value.toString());
            objectElement.appendChild(prop);
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
            if (value.convert((QVariant::Type)prop.attribute("type").toInt()))
            {
                property.write(this, value);
            }
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
        }
        component = component.nextSiblingElement("component");
    }

    return true;
}

QSet<QString> GameObject::getEditProperties()
{
    QSet<QString> set;
    set << "x" << "y" << "z" << "opacity" << "pixmapName" << "scale" << "rotation";
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
