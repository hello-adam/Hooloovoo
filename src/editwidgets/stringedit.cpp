#include "stringedit.h"
#include "ui_stringedit.h"
#include "../gamefiledialog.h"

StringEdit::StringEdit(Property *prop, QWidget *parent) :
    EditWidget(prop, parent),
    ui(new Ui::StringEdit)
{
    ui->setupUi(this);

    if (propertyValueHelperDialog() == Property::NoHelperDialog)
    {
        ui->pb_fileSelect->setVisible(false);
    }
    else
    {
        connect(ui->pb_fileSelect, SIGNAL(clicked()),
                this, SLOT(launchValueHelperDialog()));
    }

    m_helper = prop->getValueHelperDialog();
}

StringEdit::~StringEdit()
{
    delete ui;
}

void StringEdit::setValue(QVariant value)
{
    if (value.type() == QVariant::String)
    {
        ui->le_string->setText(value.toString());
    }
}

QVariant StringEdit::getValue()
{
    return QVariant(ui->le_string->text());
}

void StringEdit::launchValueHelperDialog()
{
    GameFileDialog dlg;

    if (m_helper == Property::AudioDialog)
    {
        dlg.setAcceptMode(GameFileDialog::Select);
        dlg.setFileType(FileManager::Audio);
    }
    else if (m_helper == Property::PictureDialog)
    {
        dlg.setAcceptMode(GameFileDialog::Select);
        dlg.setFileType(FileManager::Picture);
    }
    else if (m_helper == Property::GameObjectDialog)
    {
        dlg.setAcceptMode(GameFileDialog::Select);
        dlg.setFileType(FileManager::Object);
    }

    if (dlg.exec())
    {
        ui->le_string->setText(dlg.getFileName());
    }
}
