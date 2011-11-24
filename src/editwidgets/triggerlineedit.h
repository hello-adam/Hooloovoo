#ifndef TRIGGERLINEEDIT_H
#define TRIGGERLINEEDIT_H

#include <QWidget>

namespace Ui {
    class TriggerLineEdit;
}

class TriggerLineEdit : public QWidget
{
    Q_OBJECT

public:
    explicit TriggerLineEdit(QWidget *parent = 0);
    ~TriggerLineEdit();

    QString getTriggerText();
    void setTriggerText(QString text);

private:
    Ui::TriggerLineEdit *ui;

public slots:
    void launchPropertyTriggerEdit();
};

#endif // TRIGGERLINEEDIT_H
