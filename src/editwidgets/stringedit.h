#ifndef STRINGEDIT_H
#define STRINGEDIT_H

#include "editwidget.h"

namespace Ui {
    class StringEdit;
}

class StringEdit : public EditWidget
{
    Q_OBJECT

public:
    explicit StringEdit(Property *prop, QWidget *parent = 0);
    ~StringEdit();

    void setValue(QVariant value);
    QVariant getValue();

private:
    Ui::StringEdit *ui;
    Property::ValueHelperDialog m_helper;

private slots:
    void launchValueHelperDialog();
};

#endif // STRINGEDIT_H
