#include "gameobjectfilewidget.h"
#include "ui_gameobjectfilewidget.h"
#include "gamecore.h"

GameObjectFileWidget::GameObjectFileWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameObjectFileWidget)
{
    ui->setupUi(this);

    connect(&GameCore::getInstance(), SIGNAL(hasSelectedObject(bool)),
            ui->pb_saveSelectedObject, SLOT(setEnabled(bool)));
    connect(ui->pb_saveSelectedObject, SIGNAL(clicked()),
            GameCore::getInstance().getSaveSelectedObjectAction(), SLOT(trigger()));

    connect(&GameCore::getInstance(), SIGNAL(hasSelectedObject(bool)),
            ui->pb_removeObject, SLOT(setEnabled(bool)));
    connect(ui->pb_removeObject, SIGNAL(clicked()),
            GameCore::getInstance().getRemoveSelectedObjectAction(), SLOT(trigger()));

    m_model = new GameFileModel();
    m_model->setFileType(FileManager::Object);

    ui->listView->setModel(m_model);
}

GameObjectFileWidget::~GameObjectFileWidget()
{
    delete ui;
    delete m_model;
}
