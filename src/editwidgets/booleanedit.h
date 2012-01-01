#ifndef BOOLEANEDIT_H
#define BOOLEANEDIT_H

#include "editwidget.h"

namespace Ui {
    class BooleanEdit;
}

class BooleanEdit : public EditWidget
{
    Q_OBJECT

public:
    explicit BooleanEdit(Property *prop, QWidget *parent = 0);
    ~BooleanEdit();

    QVariant getValue();
    void setValue(QVariant value);

private:
    Ui::BooleanEdit *ui;
};

#endif // BOOLEANEDIT_H
