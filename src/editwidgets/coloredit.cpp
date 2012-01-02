#include "coloredit.h"
#include "ui_coloredit.h"
#include <QPalette>
#include <QColorDialog>

ColorEdit::ColorEdit(Property *prop, QWidget *parent) :
    EditWidget(prop, parent),
    ui(new Ui::ColorEdit)
{
    ui->setupUi(this);

    connect(ui->pushButton, SIGNAL(clicked()),
            this, SLOT(launchColorEditor()));
}

ColorEdit::~ColorEdit()
{
    delete ui;
}

void ColorEdit::setValue(QVariant value)
{
    if (value.type() == QVariant::Color)
    {
        QColor color = value.value<QColor>();
        setColor(color);
    }
}

QVariant ColorEdit::getValue()
{
    return QVariant(getColor());
}

QColor ColorEdit::getColor()
{
    return QColor(ui->sb_R->value(),
                  ui->sb_G->value(),
                  ui->sb_B->value());
}

void ColorEdit::setColor(QColor color)
{
    ui->pushButton->setPalette(QPalette(color));

    ui->sb_R->setValue(color.red());
    ui->sb_G->setValue(color.green());
    ui->sb_B->setValue(color.blue());
}

void ColorEdit::launchColorEditor()
{
    QColorDialog dlg;

    dlg.setCurrentColor(getColor());

    if (dlg.exec())
    {
        setColor(dlg.selectedColor());
    }
}
