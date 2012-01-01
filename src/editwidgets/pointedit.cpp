#include "pointedit.h"
#include "ui_pointedit.h"

PointEdit::PointEdit(Property *prop, QWidget *parent) :
    EditWidget(prop, parent),
    ui(new Ui::PointEdit)
{
    ui->setupUi(this);
}

PointEdit::~PointEdit()
{
    delete ui;
}

void PointEdit::setValue(QVariant value)
{
    if (value.type() == QVariant::Point || value.type() == QVariant::PointF)
    {
        ui->sb_x->setValue(value.toPointF().x());
        ui->sb_y->setValue(value.toPointF().y());
    }
}

QVariant PointEdit::getValue()
{
    return QVariant(QPointF(ui->sb_x->value(), ui->sb_y->value()));
}
