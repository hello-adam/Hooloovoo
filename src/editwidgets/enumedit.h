#ifndef ENUMEDIT_H
#define ENUMEDIT_H

#include "editwidget.h"

namespace Ui {
    class EnumEdit;
}

class EnumEdit : public EditWidget
{
    Q_OBJECT

public:
    explicit EnumEdit(Property* prop, QWidget *parent = 0);
    ~EnumEdit();

    QVariant getValue();
    void setValue(QVariant value);

private:
    Ui::EnumEdit *ui;

    QMetaEnum m_enumerator;
};

#endif // ENUMEDIT_H
