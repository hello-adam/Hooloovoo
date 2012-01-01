#ifndef DOUBLEEDIT_H
#define DOUBLEEDIT_H

#include "editwidget.h"

namespace Ui {
    class DoubleEdit;
}

class DoubleEdit : public EditWidget
{
    Q_OBJECT

public:
    explicit DoubleEdit(Property* prop, QWidget *parent = 0);
    ~DoubleEdit();

    QVariant getValue();
    void setValue(QVariant value);

private:
    Ui::DoubleEdit *ui;
};

#endif // DOUBLEEDIT_H
