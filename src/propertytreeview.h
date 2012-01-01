#ifndef PROPERTYTREEVIEW_H
#define PROPERTYTREEVIEW_H

#include <QTreeView>

class PropertyTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit PropertyTreeView(QWidget *parent = 0);

protected:
    bool edit(const QModelIndex &index, EditTrigger trigger, QEvent *event);

    void drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const;

private:
    QModelIndex m_parentIndexOfEditor;
    int m_rowOfEditor;

signals:

public slots:

protected slots:
    void closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint);

};

#endif // PROPERTYTREEVIEW_H
