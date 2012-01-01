#ifndef POINTEDIT_H
#define POINTEDIT_H

#include "editwidget.h"

namespace Ui {
    class PointEdit;
}

class PointEdit : public EditWidget
{
    Q_OBJECT

public:
    explicit PointEdit(Property* prop, QWidget *parent = 0);
    ~PointEdit();

    void setValue(QVariant value);
    QVariant getValue();

private:
    Ui::PointEdit *ui;
};

#endif // POINTEDIT_H
