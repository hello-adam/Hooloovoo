#include "propertytreeview.h"
#include "propertydelegate.h"
#include <QPainter>

PropertyTreeView::PropertyTreeView(QWidget *parent) :
    QTreeView(parent)
{
    setSelectionBehavior(QAbstractItemView::SelectItems);
    setAlternatingRowColors(false);
    setEditTriggers(QTreeView::AllEditTriggers);
    setRootIsDecorated(false);

    setItemDelegate(new PropertyDelegate(this));

//    this->setStyleSheet("QTreeView {alternate-background-color: rgba(0,0,0,35);"
//                        "background-color: rgba(255,255,255,255); }"
//                        "QTreeView::item { border: 1px solid darkgray; }");
}

bool PropertyTreeView::edit(const QModelIndex &index, EditTrigger trigger, QEvent *event)
{
    if (index.column() == 1)
    {
        PropertyDelegate* delegate = qobject_cast<PropertyDelegate*>(itemDelegate());
        delegate->addEditProperty(static_cast<Property*>(index.internalPointer()));

        m_parentIndexOfEditor = index.parent();
        m_rowOfEditor = index.row();

        this->rowsRemoved(m_parentIndexOfEditor, m_rowOfEditor, m_rowOfEditor);
    }

    return QTreeView::edit(index, trigger, event);
}

void PropertyTreeView::closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint)
{
    PropertyDelegate* delegate = qobject_cast<PropertyDelegate*>(itemDelegate());
    delegate->clearEditPropertyData();

    this->rowsRemoved(m_parentIndexOfEditor, m_rowOfEditor, m_rowOfEditor);

    emit editCompleted();

    QTreeView::closeEditor(editor, hint);
}

void PropertyTreeView::drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const
{
    painter->setPen(Qt::transparent);

    if (index.parent().isValid())
    {
        int parentColorGroup = index.parent().row() % 8;
        bool oddRow = index.row()%2 == 1;

        QColor color;

        if (oddRow)
        {
            color.setHsl(parentColorGroup*32 % 256, 255, 235);
        }
        else
        {
            color.setHsl(parentColorGroup*32 % 256, 255, 248);
        }

        painter->setBrush(color);
        painter->drawRect(options.rect);

        QTreeView::drawRow(painter, options, index);
    }
    else
    {
        painter->setBrush(Qt::darkGray);
        painter->drawRect(options.rect);

        QStyleOptionViewItem componentOption(options);

        componentOption.palette = QPalette(Qt::darkGray);
        componentOption.palette.setBrush(QPalette::Text, Qt::white);
        componentOption.palette.setBrush(QPalette::Highlight, Qt::black);

        QTreeView::drawRow(painter, componentOption, index);
    }
}
