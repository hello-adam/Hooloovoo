#include "integeredit.h"
#include "ui_integeredit.h"

IntegerEdit::IntegerEdit(Property *prop, QWidget *parent) :
    EditWidget(prop, parent),
    ui(new Ui::IntegerEdit)
{
    ui->setupUi(this);
}

IntegerEdit::~IntegerEdit()
{
    delete ui;
}

void IntegerEdit::setValue(QVariant value)
{
    if (value.type() == QVariant::Int)
    {
        ui->spinBox->setValue(value.toInt());
    }
}

QVariant IntegerEdit::getValue()
{
    return QVariant(ui->spinBox->value());
}
