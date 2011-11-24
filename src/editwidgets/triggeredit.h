#ifndef TRIGGEREDIT_H
#define TRIGGEREDIT_H

#include <QDialog>
class GameObject;
class PropertyEditWidget;

namespace Ui {
    class TriggerEdit;
}

class TriggerEdit : public QDialog
{
    Q_OBJECT

public:
    explicit TriggerEdit(QWidget *parent = 0);
    ~TriggerEdit();

    QString getTriggerText();
    void setTriggerText(QString text);

private:
    Ui::TriggerEdit *ui;
    GameObject *m_editObject;
    PropertyEditWidget *m_propertyEditWidget;

public slots:
    void componentTypeChanged();
    void propertyChanged();
};

#endif // TRIGGEREDIT_H
