#include "stringlistedit.h"
#include "ui_stringlistedit.h"
#include "../gamefiledialog.h"

StringListEdit::StringListEdit(Property *prop, QWidget *parent) :
    EditWidget(prop, parent),
    ui(new Ui::StringListEdit)
{
    ui->setupUi(this);

    connect(ui->pb_Add, SIGNAL(clicked()),
            this, SLOT(addItem()));
    connect(ui->pb_Remove, SIGNAL(clicked()),
            this, SLOT(removeSelectedItem()));
    connect(ui->pb_Edit, SIGNAL(clicked()),
            this, SLOT(editSelectedItem()));

    m_helper = prop->getValueHelperDialog();
}

StringListEdit::~StringListEdit()
{
    delete ui;
}

void StringListEdit::setValue(QVariant value)
{
    if (value.type() == QVariant::StringList)
    {
        ui->listWidget->clear();
        ui->listWidget->addItems(value.toStringList());
    }
}

QVariant StringListEdit::getValue()
{
    QStringList list;

    for (int i=0; i<ui->listWidget->count(); i++)
    {
        list << ui->listWidget->item(i)->text();
    }

    return QVariant(list);
}

void StringListEdit::addItem()
{
    GameFileDialog dlg;

    if (m_helper == Property::NoHelperDialog)
    {
        ui->listWidget->addItem("<edit me>");
        return;
    }
    else if (m_helper == Property::AudioDialog)
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
        ui->listWidget->addItem(dlg.getFileName());
    }
}

void StringListEdit::removeSelectedItem()
{
    if (ui->listWidget->currentItem())
    {
        delete ui->listWidget->takeItem(ui->listWidget->currentIndex().row());
    }
}

void StringListEdit::editSelectedItem()
{
    GameFileDialog dlg;
    Property::ValueHelperDialog dialogType = propertyValueHelperDialog();

    QListWidgetItem *item = ui->listWidget->currentItem();
    if (item)
    {
        if (dialogType == Property::NoHelperDialog)
        {
            item->setFlags(item->flags() | Qt::ItemIsEditable);
            ui->listWidget->editItem(item);
            return;
        }
        else if (dialogType == Property::AudioDialog)
        {
            dlg.setAcceptMode(GameFileDialog::Select);
            dlg.setFileType(FileManager::Audio);
        }
        else if (dialogType == Property::PictureDialog)
        {
            dlg.setAcceptMode(GameFileDialog::Select);
            dlg.setFileType(FileManager::Picture);
        }
        else if (dialogType == Property::GameObjectDialog)
        {
            dlg.setAcceptMode(GameFileDialog::Select);
            dlg.setFileType(FileManager::Object);
        }

        if (dlg.exec())
        {
            item->setText(dlg.getFileName());
        }
    }
}
