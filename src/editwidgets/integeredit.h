#ifndef INTERGEREDIT_H
#define INTERGEREDIT_H

#include "editwidget.h"

namespace Ui {
    class IntegerEdit;
}

class IntegerEdit : public EditWidget
{
    Q_OBJECT

public:
    explicit IntegerEdit(Property *prop, QWidget *parent = 0);
    ~IntegerEdit();

    QVariant getValue();
    void setValue(QVariant value);

private:
    Ui::IntegerEdit *ui;
};

#endif // INTERGEREDIT_H
