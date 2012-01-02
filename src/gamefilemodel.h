#ifndef GAMEFILEMODEL_H
#define GAMEFILEMODEL_H

#include <QAbstractItemModel>
#include "filemanager.h"

class GameFileModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit GameFileModel(QObject *parent = 0);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;

    QStringList mimeTypes() const;
    QMimeData* mimeData(const QModelIndexList &indexes) const;

private:
    FileManager::FileType m_type;

signals:

public slots:
    void setFileType(FileManager::FileType type);
    void refresh();
};

#endif // GAMEFILEMODEL_H
