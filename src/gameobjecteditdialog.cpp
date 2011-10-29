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

    m_unusedComponents = new ComponentListWidget(this);
    m_objectComponents = new ComponentListWidget(this);
    ui->UnusedComponentLayout->addWidget(m_unusedComponents);
    ui->ObjectComponentLayout->addWidget(m_objectComponents);
}

GameObjectEditDialog::~GameObjectEditDialog()
{
    delete ui;
    delete m_unusedComponents;
    delete m_objectComponents;
}

bool GameObjectEditDialog::editObject(GameObject *object)
{
    m_object = object;

    //add the game object to the stacked widget
    ui->stackedWidget->addWidget(getObjectEditWidget(object, object->getEditProperties()));
    m_stackedWidgetLookup.insert(object->objectName(), ui->stackedWidget->count()-1);
    m_objectComponents->addItem(object->objectName());
    m_objectComponents->item(0)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QList<Component*> components = object->getComponents();

    //add each of the components to the stacked widget
    foreach (Component* component, components)
    {
        ui->stackedWidget->addWidget(getObjectEditWidget(component, component->getEditProperties()));
        m_stackedWidgetLookup.insert(component->objectName(), ui->stackedWidget->count()-1);
        m_objectComponents->addItem(component->objectName());
    }

    //populate the add component combobox
    ComponentFactory factory;
    QStringList unused = factory.availableComponents();
    QList<QListWidgetItem*> used = m_objectComponents->findItems(".*", Qt::MatchRegExp);
    foreach(QListWidgetItem* item, used)
    {
        unused.removeAll(item->text());
    }

    m_unusedComponents->addItems(unused);


    connect(m_unusedComponents, SIGNAL(itemAdded(QString)),
            this, SLOT(componentRemoved(QString)));
    connect(m_objectComponents, SIGNAL(itemAdded(QString)),
            this, SLOT(componentAdded(QString)));
    connect(m_objectComponents, SIGNAL(currentRowChanged(int)),
            ui->stackedWidget, SLOT(setCurrentIndex(int)));

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
                m_editWidgets.push_back(new PropertyEditWidget(groupBox));
                m_editWidgets.back()->setProperty(property, object);
                formLayout->addRow(name, m_editWidgets.back());
            }
        }
    }

    return groupBox;
}

void GameObjectEditDialog::componentAdded(QString componentName)
{
    Component* component = 0;
    QListWidgetItem* item = 0;

    if (m_objectComponents->findItems(componentName, Qt::MatchExactly).isEmpty())
        return;

    item = m_objectComponents->findItems(componentName, Qt::MatchExactly).at(0);

    if (m_object)
        component = m_object->addComponent(componentName);

    if (component)
    {
        ui->stackedWidget->insertWidget(m_objectComponents->row(item),
                                        getObjectEditWidget(component, component->getEditProperties()));
        m_stackedWidgetLookup.insert(componentName, m_objectComponents->row(item));
    }
    else
    {
        qDebug() << "Failure Adding!";
//        m_objectComponents->removeItemWidget(item);
//        m_unusedComponents->addItem(componentName);
    }
}

void GameObjectEditDialog::componentRemoved(QString componentName)
{
    QListWidgetItem* item = 0;
    if (m_unusedComponents->findItems(componentName, Qt::MatchExactly).isEmpty())
        return;
    item = m_unusedComponents->findItems(componentName, Qt::MatchExactly).at(0);

    if (m_object)
    {
        if (m_object->removeComponent(componentName))
        {
            QWidget* componentWidget = ui->stackedWidget->widget(m_stackedWidgetLookup.value(componentName));
            ui->stackedWidget->removeWidget(componentWidget);
            //make sure that the component's edit widgets will not get written
            foreach (QObject* child, componentWidget->children())
                m_editWidgets.removeAll(qobject_cast<PropertyEditWidget*>(child));
            delete componentWidget;
            m_stackedWidgetLookup.remove(componentName);
        }
        else
        {
            qDebug() << "Failure Removing!";
//            m_unusedComponents->removeItemWidget(item);
//            m_objectComponents->addItem(componentName);
        }
    }
}
