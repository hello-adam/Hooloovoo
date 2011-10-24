#include "gameobjecteditdialog.h"
#include "ui_gameobjecteditdialog.h"
#include "propertyeditwidget.h"
#include "componentfactory.h"
#include <QHash>
#include <QBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QCheckBox>
#include <QDebug>

GameObjectEditDialog::GameObjectEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameObjectEditDialog)
{
    ui->setupUi(this);

    connect(ui->pb_AddComponent, SIGNAL(clicked()), this, SLOT(addComponent()));
    connect(ui->pb_RemoveComponent, SIGNAL(clicked()), this, SLOT(removeComponent()));
}

GameObjectEditDialog::~GameObjectEditDialog()
{
    delete ui;
}

bool GameObjectEditDialog::editObject(GameObject *object)
{
    m_object = object;

    //add the game object to the stacked widget
    ui->stackedWidget->addWidget(getObjectEditWidget(object, object->getEditProperties()));
    ui->comboBox->addItem(object->objectName());

    QList<Component*> components = object->getComponents();

    //add each of the components to the stacked widget
    foreach (Component* component, components)
    {
        ui->stackedWidget->addWidget(getObjectEditWidget(component, component->getEditProperties()));
        ui->comboBox->addItem(component->objectName());
    }

    //populate the add component combobox
    ComponentFactory factory;
    ui->cb_AddComponent->addItems(factory.availableComponents());

    if (!this->exec())
    {
        return false;
    }

    foreach (PropertyEditWidget* editWidget, m_editWidgets)
    {
        editWidget->writeProperty();
    }

    return true;
}

QWidget* GameObjectEditDialog::getObjectEditWidget(QObject* object, QSet<QString> editProperties)
{
    QGroupBox *groupBox = new QGroupBox(this);

    QFormLayout *formLayout = new QFormLayout(groupBox);
    groupBox->setLayout(formLayout);
    formLayout->setLabelAlignment(Qt::AlignVCenter | Qt::AlignRight);


    for (int i = 0; i<object->metaObject()->propertyCount(); i++)
    {
        QMetaProperty property = object->metaObject()->property(i);
        QString name = property.name();

        if (property.isValid() && property.isReadable() && property.userType())
        {
            if (name == "objectName")
            {
                QVariant value = property.read(object);
                groupBox->setTitle(value.toString());
            }
            else if (editProperties.contains(name))
            {
                m_editWidgets.push_back(new PropertyEditWidget(this));
                m_editWidgets.back()->setProperty(property, object);
                formLayout->addRow(name, m_editWidgets.back());
            }
        }
    }

    return groupBox;
}

void GameObjectEditDialog::addComponent()
{
    if (m_object)
    {
        Component* component = m_object->addComponent(ui->cb_AddComponent->currentText());

        if (component)
        {
            ui->stackedWidget->addWidget(getObjectEditWidget(component, component->getEditProperties()));
            ui->comboBox->addItem(component->objectName());
        }
    }
}

void GameObjectEditDialog::removeComponent()
{
    if (m_object && ui->comboBox->currentIndex() > 0)
    {
        if (m_object->removeComponent(ui->comboBox->currentText()))
        {
            ui->stackedWidget->removeWidget(ui->stackedWidget->widget(ui->comboBox->currentIndex()));
            ui->comboBox->removeItem(ui->comboBox->currentIndex());
            ui->comboBox->setCurrentIndex(0);
        }
    }
}
