#include "gameobjectmodel.h"
#include "gameobject.h"
#include <QApplication>

GameObjectModel::GameObjectModel(GameObject *object) :
    QAbstractItemModel(object),
    m_baseObject(object)
{
    m_rootProperty = new Property(0, "Root Object");
}

GameObjectModel::~GameObjectModel()
{
    delete m_rootProperty;
}

QVariant GameObjectModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
            return QVariant();

    Property *property = static_cast<Property*>(index.internalPointer());

    if (role == Qt::DecorationRole)
    {
        return QVariant();
    }
    else if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
    {
        if (index.column() == 0)
            return property->getDisplayName();
        if (index.column() == 1)
        {
            if (property->isComponent())
                return QVariant();
            else
                return property->getDisplayValue();
        }
    }
    else if (role == Qt::EditRole)
    {
        return QVariant();
    }
    else if (role == Qt::BackgroundRole)
    {
//        if (property->isComponent())
//            return QColor(50, 50, 50);
//        else
//        {
//            if (index.row()%2 == 1)
//            {
//                return QColor(100, 200, 100);
//            }
//            else
//            {
//                return QColor(150, 240, 150);
//            }
//        }
    }
    else if (role == Qt::ForegroundRole)
    {
    }
    else if (role == Qt::FontRole)
    {
        if (property->isComponent())
        {
            QFont componentFont;
            componentFont.setBold(true);
            return componentFont;
        }
        else
        {
            QFont normalFont;
            return normalFont;
        }
    }

    return QVariant();
}

QModelIndex GameObjectModel::index(int row, int column,
                  const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    Property *parentItem;
    Property *childItem = 0;

    if (!parent.isValid())
        parentItem = m_rootProperty;
    else
        parentItem = static_cast<Property*>(parent.internalPointer());

    if (parentItem == m_rootProperty)
    {
        childItem = m_baseObject->getComponents().at(row)->getPropertyRoot();
    }
    else
    {
        Component* component = m_baseObject->getComponents().at(parent.row());
        childItem = component->getProperties().at(row);
    }

    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

int GameObjectModel::rowCount(const QModelIndex &parent) const
{
    if (!m_baseObject)
        return 0;

    Property *parentItem;

    if (!parent.isValid())
        parentItem = m_rootProperty;
    else
        parentItem = static_cast<Property*>(parent.internalPointer());

    if (parentItem == m_rootProperty)
    {
        return m_baseObject->getComponents().count();
    }
    else
    {
        QList<Component*> components = m_baseObject->getComponents();
        foreach(Component* component, components)
        {
            if (component->objectName() == parentItem->getDisplayName())
            {
                return component->getProperties().count();
            }
        }
    }

    return 0;
}

int GameObjectModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 2;
}

QModelIndex GameObjectModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    Property *propertyChild = static_cast<Property*>(index.internalPointer());

    if (propertyChild->isComponent())
        return QModelIndex();

    QList<Component*> components = m_baseObject->getComponents();
    for (int i = 0; i< components.count(); i++)
    {
        Component* component = components.at(i);

        if (component->getProperties().contains(propertyChild))
        {
            return createIndex(i, 0, component->getPropertyRoot());
        }
    }

    return QModelIndex();
}

Qt::ItemFlags GameObjectModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flag = Qt::ItemIsEnabled;

    Property *prop =  static_cast<Property*>(index.internalPointer());

    if (index.column() == 0 && prop->isComponent())
        flag |= Qt::ItemIsSelectable;

    if (index.column() == 1 && !prop->isComponent())
        flag |= Qt::ItemIsEditable | Qt::ItemIsSelectable;

    return flag;
}

QVariant GameObjectModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if (section == 0)
            return "Property";
        else if (section == 1)
            return "Value";
    }

    return QVariant();
}

void GameObjectModel::removeComponent(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    m_baseObject->removeComponent(m_baseObject->getComponents().at(row));
    endRemoveRows();
}

void GameObjectModel::addComponent(QString name)
{
    beginInsertRows(QModelIndex(), 0, m_baseObject->getComponents().count()-1);
    m_baseObject->addComponent(name);
    endInsertRows();
}
