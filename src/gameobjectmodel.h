#ifndef GAMEOBJECTMODEL_H
#define GAMEOBJECTMODEL_H

#include <QAbstractItemModel>
class GameObject;
class Property;

class GameObjectModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit GameObjectModel(GameObject *object);
    ~GameObjectModel();

    QVariant data(const QModelIndex &index, int role) const;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;


    QModelIndex parent(const QModelIndex &index) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    void removeComponent(int row);
    void addComponent(QString name);

private:
    GameObject* m_baseObject;
    Property* m_rootProperty;

signals:

public slots:

};

#endif // GAMEOBJECTMODEL_H
