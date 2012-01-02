#include "gamefilemodel.h"
#include <QIcon>

GameFileModel::GameFileModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    m_type = FileManager::Game;

    connect(&FileManager::getInstance(), SIGNAL(gameObjectAddedOrRemoved()),
            this, SLOT(refresh()));
}

Qt::ItemFlags GameFileModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index)

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
}

QVariant GameFileModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DecorationRole)
    {
        if (m_type == FileManager::Picture)
        {
            QPixmap picture(FileManager::getInstance().getActivePathForType(m_type) +
                            FileManager::getInstance().getAvailableFilesOfType(m_type).at(index.row()));

            return QVariant(QIcon(picture));
        }
        else if (m_type == FileManager::Object)
        {
            QDomElement object = FileManager::getInstance().loadGameObject(FileManager::getInstance().getAvailableFilesOfType(m_type).at(index.row()));
            QDomElement pixmapFile = object.firstChildElement("pixmapFileName");
            if (!pixmapFile.isNull())
            {
                QPixmap picture = QPixmap(FileManager::getInstance().getActivePathForType(FileManager::Picture) +
                                          pixmapFile.attribute("value"));

                if (picture.isNull())
                {
                    picture = QPixmap(42, 42);
                    QDomElement defaultColor = object.firstChildElement("defaultColor");
                    if (!defaultColor.isNull())
                    {
                        picture.fill(QColor(defaultColor.attribute("value").mid(1).toUInt(0, 16)));
                    }
                }
                return QVariant(QIcon(picture));
            }
            else
            {
                return QVariant();
            }
        }
        else
        {
            return QVariant();
        }
    }
    else if (role == Qt::DisplayRole || role == Qt::EditRole || role == Qt::ToolTipRole)
    {
        return FileManager::getInstance().getAvailableFilesOfType(m_type).at(index.row());
    }
    else
    {
        return QVariant();
    }
}

QVariant GameFileModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

int GameFileModel::rowCount(const QModelIndex &parent) const
{
    return FileManager::getInstance().getAvailableFilesOfType(m_type).count();
}

int GameFileModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QModelIndex GameFileModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return createIndex(row, column, 0);
}

QModelIndex GameFileModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

QStringList GameFileModel::mimeTypes() const
 {
     QStringList types;
     types << "application/vnd.text.list";
     return types;
 }

QMimeData* GameFileModel::mimeData(const QModelIndexList &indexes) const
 {
     QMimeData *mimeData = new QMimeData();
     QByteArray encodedData;

     QDataStream stream(&encodedData, QIODevice::WriteOnly);

     foreach (const QModelIndex &index, indexes) {
         if (index.isValid()) {
             QString text = data(index, Qt::DisplayRole).toString();
             stream << text;
         }
     }

     mimeData->setData("application/vnd.text.list", encodedData);
     return mimeData;
 }

void GameFileModel::setFileType(FileManager::FileType type)
{
    m_type = type;
    refresh();
}

void GameFileModel::refresh()
{
    beginResetModel();
    endResetModel();
}
