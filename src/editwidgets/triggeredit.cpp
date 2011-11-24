#include "triggeredit.h"
#include "ui_triggeredit.h"
#include "../componentfactory.h"
#include "../gameobject.h"
#include "../propertyeditwidget.h"
#include <QDebug>

TriggerEdit::TriggerEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TriggerEdit)
{
    ui->setupUi(this);

    m_editObject = new GameObject();

    m_propertyEditWidget = 0;

    ComponentFactory factory;
    ui->cb_ComponentType->addItem("<No Component Edit>");
    ui->cb_ComponentType->addItem("Game Object");
    ui->cb_ComponentType->addItems(factory.availableComponents());

    componentTypeChanged();

    connect(ui->cb_ComponentType, SIGNAL(currentIndexChanged(int)),
            this, SLOT(componentTypeChanged()));

    connect(ui->cb_PropertyName, SIGNAL(currentIndexChanged(int)),
            this, SLOT(propertyChanged()));
}

TriggerEdit::~TriggerEdit()
{
    delete ui;

    delete m_editObject;
}

QString TriggerEdit::getTriggerText()
{
    QString text = "";
    if (!ui->cb_ComponentTag->currentText().isEmpty())
        text += "Tag(" + ui->cb_ComponentTag->currentText() + ")";

    if (m_propertyEditWidget
            && !ui->cb_ComponentTag->currentText().isEmpty()
            && !ui->cb_PropertyName->currentText().isEmpty())
    {
        text += "Property(" + ui->cb_ComponentType->currentText() + ",";
        text += ui->cb_PropertyName->currentText() + ",";
        //text += Edit Type + ",";
        text += m_propertyEditWidget->getValue().toString() + ")";
    }

    return text;
}

void TriggerEdit::setTriggerText(QString text)
{
    if (text.contains(QRegExp("Tag(.*)")))
    {
        int start = text.indexOf("Tag(")+4;
        int end = text.indexOf(")", start);
        QString tag = text.mid(start, end-start);

        text.remove("Tag(.*)");

        ui->cb_ComponentTag->lineEdit()->setText(tag);
    }

    if (text.contains(QRegExp("Property(.*)")))
    {
        int start = text.indexOf("Property(")+9;
        int end = text.indexOf(")", start);
        QStringList args = text.mid(start, end-start).split(',', QString::KeepEmptyParts);

        text.remove("Property(.*)");

        //TODO: make it fill in the current property specs
    }
}

void TriggerEdit::componentTypeChanged()
{
    disconnect(ui->cb_PropertyName, SIGNAL(currentIndexChanged(int)),
            this, SLOT(propertyChanged()));

    if (m_propertyEditWidget)
    {
        delete m_propertyEditWidget;
        m_propertyEditWidget = 0;
    }

    QString componentName = ui->cb_ComponentType->currentText();

    if (componentName == "Game Object")
    {
        QStringList properties = m_editObject->getEditProperties().toList();
        ui->cb_PropertyName->clear();
        ui->cb_PropertyName->addItems(properties);
    }
    else
    {
        ComponentFactory factory;
        Component* sampleComponent = factory.createComponent(m_editObject, componentName);

        if (!sampleComponent)
        {
            ui->cb_PropertyName->clear();
            return;
        }

        QStringList properties = sampleComponent->getEditProperties().toList();
        ui->cb_PropertyName->clear();
        ui->cb_PropertyName->addItems(properties);
    }

    propertyChanged();
    connect(ui->cb_PropertyName, SIGNAL(currentIndexChanged(int)),
            this, SLOT(propertyChanged()));
}

void TriggerEdit::propertyChanged()
{
    if (m_propertyEditWidget)
    {
        delete m_propertyEditWidget;
        m_propertyEditWidget = 0;
    }

    QObject *object;

    QString componentName = ui->cb_ComponentType->currentText();
    if (componentName == "Game Object")
    {
        object = m_editObject;
    }
    else
    {
        ComponentFactory factory;
        Component* sampleComponent = factory.createComponent(m_editObject, componentName);

        if (!sampleComponent)
            return;

        object = sampleComponent;
    }


    for (int i = 0; i<object->metaObject()->propertyCount(); i++)
    {
        QMetaProperty property = object->metaObject()->property(i);
        QString name = property.name();

        if (property.isValid() && property.isReadable() && property.userType())
        {
            if (name == ui->cb_PropertyName->currentText())
            {
                qDebug() << "setting up property widget " << name;
                m_propertyEditWidget = new PropertyEditWidget();
                m_propertyEditWidget->setProperty(property, object);
                ui->lo_editLayout->addWidget(m_propertyEditWidget);
                break;
            }
        }
    }
}
