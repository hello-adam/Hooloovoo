#ifndef PROPERTYDELEGATE_H
#define PROPERTYDELEGATE_H

#include <QStyledItemDelegate>
#include "property.h"

class PropertyDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit PropertyDelegate(QObject *parent = 0);
    virtual ~PropertyDelegate();

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor,  const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

signals:
    void adjustIndexForEditor(QModelIndex);

private:
    QSet<Property*> m_editProperties;

public slots:
    void clearEditPropertyData() {m_editProperties.clear();}
    void addEditProperty(Property* prop) {m_editProperties.insert(prop);}
};

#endif // PROPERTYDELEGATE_H
