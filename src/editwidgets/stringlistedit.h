#ifndef STRINGLISTEDIT_H
#define STRINGLISTEDIT_H

#include "editwidget.h"

namespace Ui {
    class StringListEdit;
}

class StringListEdit : public EditWidget
{
    Q_OBJECT

public:
    explicit StringListEdit(Property *prop, QWidget *parent = 0);
    ~StringListEdit();

    QVariant getValue();
    void setValue(QVariant value);

private:
    Ui::StringListEdit *ui;
    Property::ValueHelperDialog m_helper;

private slots:
    void addItem();
    void removeSelectedItem();
    void editSelectedItem();
};

#endif // STRINGLISTEDIT_H
