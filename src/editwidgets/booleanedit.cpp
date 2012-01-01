#include "booleanedit.h"
#include "ui_booleanedit.h"

BooleanEdit::BooleanEdit(Property *prop, QWidget *parent) :
    EditWidget(prop, parent),
    ui(new Ui::BooleanEdit)
{
    ui->setupUi(this);
}

BooleanEdit::~BooleanEdit()
{
    delete ui;
}

void BooleanEdit::setValue(QVariant value)
{
    if (value.type() == QVariant::Bool)
    {
        ui->checkBox->setChecked(value.toBool());
    }
}

QVariant BooleanEdit::getValue()
{
    return QVariant(ui->checkBox->isChecked());
}
