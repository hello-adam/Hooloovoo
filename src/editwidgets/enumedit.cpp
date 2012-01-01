#include "enumedit.h"
#include "ui_enumedit.h"

EnumEdit::EnumEdit(Property *prop, QWidget *parent) :
    EditWidget(prop, parent),
    ui(new Ui::EnumEdit)
{
    ui->setupUi(this);

    m_enumerator = prop->getProperty().enumerator();
    for (int i=0; i<m_enumerator.keyCount(); i++)
    {
        ui->comboBox->addItem(m_enumerator.key(i));
    }
}

EnumEdit::~EnumEdit()
{
    delete ui;
}

void EnumEdit::setValue(QVariant value)
{
    int index = ui->comboBox->findText(m_enumerator.valueToKey(*reinterpret_cast<const int *>(value.constData())));
    if (index == -1)
    {
        qDebug() << "Error reading enum";
        return;
    }
    ui->comboBox->setCurrentIndex(index);
}

QVariant EnumEdit::getValue()
{
    QVariant value;

    value.setValue(m_enumerator.keyToValue(qPrintable(ui->comboBox->currentText())));

    return value;
}
