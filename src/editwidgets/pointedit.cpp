#include "pointedit.h"
#include "ui_pointedit.h"

PointEdit::PointEdit(QPointF point, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PointEdit)
{
    ui->setupUi(this);

    this->setPoint(point);
}

PointEdit::~PointEdit()
{
    delete ui;
}

void PointEdit::setPoint(QPointF point)
{
    ui->sb_x->setValue(point.x());
    ui->sb_y->setValue(point.y());
}

QPointF PointEdit::getPoint()
{
    return QPointF(ui->sb_x->value(), ui->sb_y->value());
}
