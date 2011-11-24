#include "triggerlineedit.h"
#include "ui_triggerlineedit.h"
#include <QDialog>
#include <QLineEdit>
#include <QVBoxLayout>
#include "triggeredit.h"

TriggerLineEdit::TriggerLineEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TriggerLineEdit)
{
    ui->setupUi(this);

    connect(ui->pb_propertyEdit, SIGNAL(clicked()),
            this, SLOT(launchPropertyTriggerEdit()));
}

TriggerLineEdit::~TriggerLineEdit()
{
    delete ui;
}

QString TriggerLineEdit::getTriggerText()
{
    return ui->cb_TriggerBox->currentText();
}

void TriggerLineEdit::setTriggerText(QString text)
{
    ui->cb_TriggerBox->lineEdit()->setText(text);
}

void TriggerLineEdit::setAdvancedEditButton(bool enable)
{
    if (enable)
    {
        ui->pb_propertyEdit->setVisible(true);
    }
    else
    {
        ui->pb_propertyEdit->setVisible(false);
    }
}

void TriggerLineEdit::launchPropertyTriggerEdit()
{
    TriggerEdit editor;

    if (editor.exec())
    {
        ui->cb_TriggerBox->lineEdit()->setText(editor.getTriggerText());
    }
}
