#include "editwidget.h"

EditWidget::EditWidget(Property *prop, QWidget *parent) :
    QWidget(parent),
    m_property(prop)
{
    this->setAutoFillBackground(true);
}
