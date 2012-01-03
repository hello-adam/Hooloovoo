#include "gameobjecteditwidget.h"
#include "ui_gameobjecteditwidget.h"
#include "componentfactory.h"

GameObjectEditWidget::GameObjectEditWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameObjectEditWidget)
{
    ui->setupUi(this);

    m_propertyTreeView = new PropertyTreeView(this);
    this->layout()->addWidget(m_propertyTreeView);

    m_selectedComponentRow = 0;

    m_gameObject = 0;

    connect(ui->cb_components, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(checkAddSelection(QString)));
    connect(m_propertyTreeView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(checkRemoveSelection(QModelIndex)));

    connect(ui->pb_addSelected, SIGNAL(clicked()),
            this, SLOT(addSelectedComponent()));
    connect(ui->pb_removeSelected, SIGNAL(clicked()),
            this, SLOT(removeSelectedComponent()));

    connect(m_propertyTreeView, SIGNAL(editCompleted()),
            this, SIGNAL(editCompleted()));

    setupComboBoxAndButtons();
}

GameObjectEditWidget::~GameObjectEditWidget()
{
    delete ui;
}

void GameObjectEditWidget::setupComboBoxAndButtons()
{
    ui->cb_components->clear();
    ui->pb_addSelected->setEnabled(false);
    ui->pb_removeSelected->setEnabled(false);
    ui->pb_addSelected->setText("Add Selected Component");
    ui->pb_removeSelected->setText("Remove Selected Component");

    if (m_gameObject)
    {
        ui->cb_components->addItem("Game Object");
        ui->cb_components->addItems(ComponentFactory::availableComponents());
        ui->cb_components->setCurrentIndex(0);
    }
}

void GameObjectEditWidget::setGameObject(GameObject* gameObject)
{
    m_gameObject = gameObject;

    if (gameObject)
    {
        m_propertyTreeView->setModel(gameObject->getModel());
        m_propertyTreeView->expandToDepth(0);
    }
    else
    {
        m_propertyTreeView->setModel(0);
    }

    setupComboBoxAndButtons();
}

void GameObjectEditWidget::checkRemoveSelection(QModelIndex selection)
{
    ui->pb_removeSelected->setEnabled(false);
    ui->pb_removeSelected->setText("Remove Selected Component");

    if (!selection.isValid())
        return;

    Property *prop = static_cast<Property*>(selection.internalPointer());

    if (prop->isComponent())
    {
        m_selectedComponentRow = selection.row();
        ui->pb_removeSelected->setEnabled(true);
        ui->pb_removeSelected->setText("Remove \"" + prop->getDisplayName() + "\"");
    }
}

void GameObjectEditWidget::checkAddSelection(QString componentName)
{
    ui->pb_addSelected->setEnabled(false);
    ui->pb_addSelected->setText("Add Selected Component");

    if (!m_gameObject)
        return;

    if (componentName == "Game Object")
        return;

    QList<Component*> components = m_gameObject->getComponents(componentName);

    if (!components.isEmpty())
    {
        if (!components.first()->allowMultipleComponents())
            return;
    }

    ui->pb_addSelected->setEnabled(true);
    ui->pb_addSelected->setText("Add \"" + componentName + "\"");
}

void GameObjectEditWidget::removeSelectedComponent()
{
    if (!m_gameObject)
        return;

    if (m_selectedComponentRow < 1 || m_selectedComponentRow >= m_gameObject->getComponents().count())
        return;

    m_gameObject->getModel()->removeComponent(m_selectedComponentRow);


    //TODO: make it so that this does not need to be reset so harshly after component removal...
    setupComboBoxAndButtons();
}

void GameObjectEditWidget::addSelectedComponent()
{
    if (!m_gameObject)
        return;

    m_gameObject->getModel()->addComponent(ui->cb_components->currentText());
    m_propertyTreeView->expandToDepth(0);
}
