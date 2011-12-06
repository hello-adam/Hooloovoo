#include "gameobjecteditdialog.h"
#include "ui_gameobjecteditdialog.h"
#include "propertyeditwidget.h"
#include "componentfactory.h"
#include "componenteditwidget.h"
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
#include <QScrollArea>
#include <QDebug>

GameObjectEditDialog::GameObjectEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameObjectEditDialog)
{
    ui->setupUi(this);

    m_causeAndEffectWidget = new CauseAndEffectEditWidget();
    ui->causeAndEffectLayout->addWidget(m_causeAndEffectWidget);

    connect(ui->pb_AddSelected, SIGNAL(clicked()), this, SLOT(addSelectedComponent()));
}

GameObjectEditDialog::~GameObjectEditDialog()
{
    if (m_causeAndEffectWidget)
        delete m_causeAndEffectWidget;

    delete ui;

    //qDeleteAll(this->children());
}

bool GameObjectEditDialog::editObject(GameObject *object)
{
    m_object = object;

    //add the game object to the stacked widget
    ui->sw_ComponentEditor->addWidget(getObjectEditWidget(object, object->getEditProperties()));
    ui->lw_components->addItem(object->objectName());

    //add each of the component pages to the stacked widget
    ComponentFactory factory;
    QStringList available = factory.availableComponents();
    foreach(QString name, available)
    {
        QGroupBox *groupBox = new QGroupBox(this);
        QVBoxLayout *gLayout = new QVBoxLayout();
        groupBox->setLayout(gLayout);
        groupBox->setTitle(name);
        groupBox->setFlat(true);
        gLayout->setMargin(0);
        ui->sw_ComponentEditor->addWidget(groupBox);

        QScrollArea *scrollArea = new QScrollArea(groupBox);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        scrollArea->setWidgetResizable(true);
        gLayout->addWidget(scrollArea);

        QWidget *baseWidget = new QWidget(this);
        baseWidget->setMaximumWidth(300);
        QVBoxLayout *bLayout = new QVBoxLayout();
        bLayout->setMargin(3);
        baseWidget->setLayout(bLayout);
        scrollArea->setWidget(baseWidget);
        baseWidget->show();

        ui->lw_components->addItem(name);
        m_componentWidgetLookup.insert(name, baseWidget);
    }

    //add each of the objects components to their corresponding component pages in the stacked widget
    QList<Component*> components = object->getComponents();
    foreach (Component* component, components)
    {
        //add property data
        QWidget *widget = m_componentWidgetLookup.value(component->objectName(), 0);

        if (widget)
        {
            ComponentEditWidget* editWidget = new ComponentEditWidget();
            editWidget->setComponent(component);
            if (widget->layout()->count() > 0)
            {
                QFrame *line = new QFrame();
                line->setFrameShape(QFrame::HLine);
                line->setFrameShadow(QFrame::Sunken);
                widget->layout()->addWidget(line);
                connect (editWidget, SIGNAL(destroyed()), line, SLOT(deleteLater()));
            }
            widget->layout()->addWidget(editWidget);
            connect(this, SIGNAL(accepted()), editWidget, SLOT(saveChanges()));
        }
    }

    resetCauseEffectWidget();

    if (this->exec())
    {
        m_object->initiateObject();
        return true;
    }
    else
    {
        return false;
    }
}

QWidget* GameObjectEditDialog::getObjectEditWidget(QObject* object, QSet<QString> editProperties)
{
    QGroupBox *groupBox = new QGroupBox(this);
    groupBox->setFlat(true);

    QFormLayout *formLayout = new QFormLayout();
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
                PropertyEditWidget* editWidget = new PropertyEditWidget(this);
                editWidget->setProperty(property, object);
                connect(this, SIGNAL(accepted()), editWidget, SLOT(writeProperty()));
                formLayout->addRow(name, editWidget);
            }
        }
    }

    return groupBox;
}

void GameObjectEditDialog::addSelectedComponent()
{
    Component* component = m_object->addComponent(ui->lw_components->currentItem()->text());

    if (!component)
        return;

    QWidget *widget = m_componentWidgetLookup.value(component->objectName(), 0);

    if (widget)
    {
        ComponentEditWidget* editWidget = new ComponentEditWidget();
        editWidget->setComponent(component);
        if (widget->layout()->count() > 0)
        {
            QFrame *line = new QFrame();
            line->setFrameShape(QFrame::HLine);
            line->setFrameShadow(QFrame::Sunken);
            widget->layout()->addWidget(line);
            connect (editWidget, SIGNAL(destroyed()), line, SLOT(deleteLater()));
        }
        widget->layout()->addWidget(editWidget);
        connect(this, SIGNAL(accepted()), editWidget, SLOT(saveChanges()));
        connect(editWidget, SIGNAL(componentRemoved()), this, SLOT(resetCauseEffectWidget()));
    }

    resetCauseEffectWidget();
}

void GameObjectEditDialog::resetCauseEffectWidget()
{
    if (!m_object)
        return;

    if (m_causeAndEffectWidget)
    {
        ui->causeAndEffectLayout->removeWidget(m_causeAndEffectWidget);
        delete m_causeAndEffectWidget;
    }
    m_causeAndEffectWidget = new CauseAndEffectEditWidget();
    ui->causeAndEffectLayout->addWidget(m_causeAndEffectWidget);

    //add game object cause and effect info
    QStringList objCauses = m_object->getCauseList();
    foreach(QString cause, objCauses)
    {
        m_causeAndEffectWidget->addCause(m_object, cause);
    }

    QStringList objEffects = m_object->getEffectList();
    foreach(QString effect, objEffects)
    {
        m_causeAndEffectWidget->addEffect(m_object, effect);
    }

    QList<Component*> components = m_object->getComponents();
    foreach (Component* component, components)
    {
        //add cause and effect data
        QStringList causes = component->getCauseList();
        foreach(QString cause, causes)
        {
            m_causeAndEffectWidget->addCause(component, cause);
        }

        QStringList effects = component->getEffectList();
        foreach(QString effect, effects)
        {
            m_causeAndEffectWidget->addEffect(component, effect);
        }
    }
}
