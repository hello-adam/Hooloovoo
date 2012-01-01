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
        m_color = value.value<QColor>();

        ui->pushButton->setPalette(QPalette(m_color));
    }
}

QVariant ColorEdit::getValue()
{
    return QVariant(m_color);
}

void ColorEdit::launchColorEditor()
{
    QColorDialog dlg;

    dlg.setCurrentColor(m_color);

    if (dlg.exec())
    {
        m_color = dlg.selectedColor();
        ui->pushButton->setPalette(QPalette(m_color));
    }
}
