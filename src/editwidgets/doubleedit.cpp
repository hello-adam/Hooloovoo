#include "doubleedit.h"
#include "ui_doubleedit.h"

DoubleEdit::DoubleEdit(Property *prop, QWidget *parent) :
    EditWidget(prop, parent),
    ui(new Ui::DoubleEdit)
{
    ui->setupUi(this);
}

DoubleEdit::~DoubleEdit()
{
    delete ui;
}

void DoubleEdit::setValue(QVariant value)
{
    if (value.type() == QVariant::Double)
    {
        ui->doubleSpinBox->setValue(value.toDouble());
    }
}

QVariant DoubleEdit::getValue()
{
    return QVariant(ui->doubleSpinBox->value());
}
