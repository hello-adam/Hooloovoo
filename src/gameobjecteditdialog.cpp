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

    connect(ui->buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()),
            this, SLOT(saveChanges()));
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()),
            this, SLOT(saveChanges()));
}

GameObjectEditDialog::~GameObjectEditDialog()
{
    if (m_causeAndEffectWidget)
        delete m_causeAndEffectWidget;

    qDeleteAll(m_componentEditWidgets);

    delete ui;
}

bool GameObjectEditDialog::editObject(GameObject *object)
{
    m_object = object;

    //add each of the component pages to the stacked widget
    ComponentFactory factory;
    QStringList available;
    available << "Game Object";
    available << factory.availableComponents();
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

    //add the game object to the stacked widget
    addComponentEditWidget(m_object);

    //add each of the objects components to their corresponding component pages in the stacked widget
    QList<Component*> components = object->getComponents();
    foreach (Component* component, components)
    {
        addComponentEditWidget(component);
    }

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

void GameObjectEditDialog::addSelectedComponent()
{
    Component* component = m_object->addComponent(ui->lw_components->currentItem()->text());

    if (!component)
        return;

    addComponentEditWidget(component);
}

void GameObjectEditDialog::addComponentEditWidget(Component *component)
{
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
        connect(editWidget, SIGNAL(componentRemoved(ComponentEditWidget*)), this, SLOT(removeComponentEditWidget(ComponentEditWidget*)));

        m_componentEditWidgets.insert(editWidget);

        resetCauseEffectWidget();
    }
}

void GameObjectEditDialog::removeComponentEditWidget(ComponentEditWidget *editWidget)
{
    if (!editWidget)
        return;

    m_componentEditWidgets.remove(editWidget);
    editWidget->deleteLater();

    resetCauseEffectWidget();
}

void GameObjectEditDialog::saveChanges()
{
    foreach(ComponentEditWidget* editWidget, m_componentEditWidgets)
    {
        editWidget->saveChanges();
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
