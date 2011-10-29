#include "componentlistwidget.h"
#include <QDropEvent>
#include <QDebug>

ComponentListWidget::ComponentListWidget(QWidget *parent) :
    QListWidget(parent)
{
    this->setDragDropMode(QListWidget::DragDrop);
    this->setAcceptDrops(true);
    this->setDefaultDropAction(Qt::MoveAction);
    this->setMaximumWidth(130);
    this->setEditTriggers(QListWidget::NoEditTriggers);
    this->setAlternatingRowColors(true);
}

void ComponentListWidget::dropEvent(QDropEvent *event)
{
    QByteArray encodedData = event->encodedData(event->format());
    QDataStream stream( &encodedData, QIODevice::ReadOnly );
    int row;
    int column;
    QMap<int, QVariant> dataMap;
    stream >> row >> column >> dataMap;
    QString displayRole = dataMap.value(0).toString();

    if (!this->findItems(displayRole, Qt::MatchExactly).isEmpty())
    {
        this->insertItem(this->row(this->findItems(displayRole, Qt::MatchExactly).at(0)), displayRole);
        event->acceptProposedAction();
        return;
    }

    this->addItem(displayRole);
    emit itemAdded(displayRole);

    event->acceptProposedAction();
}
