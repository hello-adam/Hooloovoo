#include "propertydelegate.h"
#include "editwidgets/editwidget.h"

PropertyDelegate::PropertyDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

PropertyDelegate::~PropertyDelegate()
{

}

QWidget* PropertyDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem& option , const QModelIndex & index ) const
{
    Q_UNUSED(option)

    QWidget* editor = 0;
    Property* p = static_cast<Property*>(index.internalPointer());

    if (p->isComponent())
        return 0;

    editor = p->createEditorWidget();

    if (editor)
    {
        editor->setParent(parent);
    }

    return editor;
}

void PropertyDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    Q_UNUSED(index)

    if (qobject_cast<EditWidget*>(editor))
    {
        EditWidget *editWidget = qobject_cast<EditWidget*>(editor);
        editWidget->readProperty();
    }
}

void PropertyDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (qobject_cast<EditWidget*>(editor))
    {
        EditWidget *editWidget = qobject_cast<EditWidget*>(editor);
        editWidget->writeProperty();
    }
}

void PropertyDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex& index ) const
{
    QStyledItemDelegate::updateEditorGeometry(editor, option, index);
}

QSize PropertyDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);

    Property *p = static_cast<Property*>(index.internalPointer());

    if (m_editProperties.contains(p))
    {
        if (p->isComponent())
            return size;

        QWidget *editor = p->createEditorWidget();
        size.setHeight(editor->sizeHint().height());

        delete editor;
    }

    return size;
}

bool PropertyDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    //m_editProperties.insert(static_cast<Property*>(index.internalPointer()));

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
