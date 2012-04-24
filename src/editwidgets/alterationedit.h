#ifndef ALTERATIONEDIT_H
#define ALTERATIONEDIT_H

#include "editwidget.h"
#include "componentalteration.h"

namespace Ui {
    class AlterationEdit;
}

class AlterationEdit : public EditWidget
{
    Q_OBJECT

public:
    explicit AlterationEdit(ComponentAlteration* prop, QWidget *parent = 0);
    ~AlterationEdit();

    QVariant getValue();
    void setValue(QVariant value);

private:
    Ui::AlterationEdit *ui;
    ComponentAlteration *m_alteration;
    EditWidget* m_valueWidget;

public slots:
    void componentTypeChanged();
    void propertyChanged();
};

#endif // ALTERATIONEDIT_H
