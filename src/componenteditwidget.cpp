#include "componenteditwidget.h"
#include "ui_componenteditwidget.h"
#include "gameobject.h"

ComponentEditWidget::ComponentEditWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComponentEditWidget)
{
    ui->setupUi(this);

    connect(ui->pb_remove, SIGNAL(clicked()), this, SLOT(removeComponent()));
}

ComponentEditWidget::~ComponentEditWidget()
{
    qDeleteAll(m_propertyEditWidgets);
    m_propertyEditWidgets.clear();
    delete ui;
}

bool ComponentEditWidget::setComponent(Component *component)
{
    if (!component)
        return false;

    m_component = component;

    for (int i = 0; i<component->metaObject()->propertyCount(); i++)
    {
        QMetaProperty property = component->metaObject()->property(i);
        QString name = property.name();

        if (property.isValid() && property.isReadable() && property.userType())
        {
            if (component->getEditProperties().contains(name))
            {
                m_propertyEditWidgets.push_back(new PropertyEditWidget(this));
                m_propertyEditWidgets.back()->setProperty(property, component);
                ui->formLayout->addRow(name, m_propertyEditWidgets.back());
            }
        }
    }

    return true;
}

void ComponentEditWidget::removeComponent()
{
    GameObject* gameObject = m_component->getParentObject();

    gameObject->removeComponent(m_component);
    m_component = 0;

    this->deleteLater();
}

void ComponentEditWidget::saveChanges()
{
    foreach (PropertyEditWidget* edit, m_propertyEditWidgets)
    {
        edit->writeProperty();
    }
}
