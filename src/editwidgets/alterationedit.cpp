#include "alterationedit.h"
#include "ui_alterationedit.h"
#include "../gameobject.h"
#include "../componentfactory.h"

AlterationEdit::AlterationEdit(ComponentAlteration *prop, QWidget *parent) :
    EditWidget(prop, parent),
    ui(new Ui::AlterationEdit)
{
    ui->setupUi(this);

    m_alteration = prop;
    m_valueWidget = 0;

    ComponentFactory factory;
    ui->cb_componentName->addItem("<No Component Edit>");
    ui->cb_componentName->addItem("Game Object");
    ui->cb_componentName->addItems(factory.availableComponents());

    componentTypeChanged();

    connect(ui->cb_componentName, SIGNAL(currentIndexChanged(int)),
            this, SLOT(componentTypeChanged()));

    connect(ui->cb_propertyName, SIGNAL(currentIndexChanged(int)),
            this, SLOT(propertyChanged()));
}

AlterationEdit::~AlterationEdit()
{
    delete ui;
}

void AlterationEdit::setValue(QVariant value)
{
    disconnect(ui->cb_propertyName, SIGNAL(currentIndexChanged(int)),
            this, SLOT(propertyChanged()));

    if (value.type() == QVariant::String)
    {
        qDebug() << "setting alteration";
        m_alteration->setValue(value);
        ui->cb_tag->addItem(m_alteration->getTag());
        ui->cb_tag->setCurrentIndex(ui->cb_tag->findText(m_alteration->getTag()));
        if (ui->cb_componentName->findText(m_alteration->getName()) >= 0)
        {
            ui->cb_componentName->setCurrentIndex(ui->cb_componentName->findText(m_alteration->getName()));
        }
        if (ui->cb_propertyName->findText(m_alteration->getPropertyName()) >= 0)
        {
            ui->cb_propertyName->setCurrentIndex(ui->cb_propertyName->findText(m_alteration->getPropertyName()));
        }
        propertyChanged();
        if (m_valueWidget)
        {
            qDebug() << "setting value after widget setup";
            m_alteration->setValue(value);
            m_valueWidget->readProperty();
        }
    }

    connect(ui->cb_propertyName, SIGNAL(currentIndexChanged(int)),
            this, SLOT(propertyChanged()));
}

QVariant AlterationEdit::getValue()
{
    m_alteration->setTag(ui->cb_tag->currentText());
    m_alteration->setName(ui->cb_componentName->currentText());
    m_alteration->setPropertyName(ui->cb_propertyName->currentText());
    if (m_valueWidget)
        m_alteration->setPropertyValue(m_valueWidget->getValue());

    return QVariant(m_alteration->dataToString());
}


void AlterationEdit::componentTypeChanged()
{
    disconnect(ui->cb_propertyName, SIGNAL(currentIndexChanged(int)),
            this, SLOT(propertyChanged()));

    if (m_valueWidget)
    {
        delete m_valueWidget;
        m_valueWidget = 0;
    }

    QString componentName = ui->cb_componentName->currentText();

    ui->cb_propertyName->clear();
    Component* sampleComponent;
    if (componentName == "Game Object")
    {
        sampleComponent = new GameObject();
    }
    else
    {
        ComponentFactory factory;
        sampleComponent = factory.createComponent(0, componentName);
    }

    if (sampleComponent)
    {
        QList<Property*> properties = sampleComponent->getProperties();
        foreach(Property* prop, properties)
        {
            ui->cb_propertyName->addItem(prop->getDisplayName());
        }
    }

    delete sampleComponent;

    propertyChanged();
    connect(ui->cb_propertyName, SIGNAL(currentIndexChanged(int)),
            this, SLOT(propertyChanged()));
}

void AlterationEdit::propertyChanged()
{
    if (m_valueWidget)
    {
        delete m_valueWidget;
        m_valueWidget = 0;
    }

    QString componentName = ui->cb_componentName->currentText();
    Component* sampleComponent;
    if (componentName == "Game Object")
    {
        sampleComponent = new GameObject();
    }
    else
    {
        ComponentFactory factory;
        sampleComponent = factory.createComponent(0, componentName);
    }

    if (!sampleComponent)
    {
        return;
    }

    QList<Property*> properties = sampleComponent->getProperties();
    foreach(Property* prop, properties)
    {
        if (prop->getDisplayName() == ui->cb_propertyName->currentText())
        {
            m_alteration->getValueProperty()->setValue(prop->getValue());
            m_alteration->getValueProperty()->setValueHelperDialog(prop->getValueHelperDialog());

            QWidget* widget = m_alteration->getValueProperty()->createEditorWidget();
            m_valueWidget = qobject_cast<EditWidget*>(widget);
            if (m_valueWidget)
            {
                qDebug() << "has value widget";
                ui->lo_editLayout->addWidget(m_valueWidget);
            }
            else
                delete widget;
            break;
        }
    }

    delete sampleComponent;
}
