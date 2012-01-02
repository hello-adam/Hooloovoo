#ifndef COLOREDIT_H
#define COLOREDIT_H

#include "editwidget.h"

namespace Ui {
    class ColorEdit;
}

class ColorEdit : public EditWidget
{
    Q_OBJECT

public:
    explicit ColorEdit(Property *prop, QWidget *parent = 0);
    ~ColorEdit();

    QVariant getValue();
    void setValue(QVariant value);

private:
    QColor getColor();
    void setColor(QColor color);
    Ui::ColorEdit *ui;

private slots:
    void launchColorEditor();
};

#endif // COLOREDIT_H
