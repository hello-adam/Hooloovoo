#include "leveldatadialog.h"
#include "ui_leveldatadialog.h"

#include "physicsmanager.h"

LevelDataDialog::LevelDataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LevelDataDialog)
{
    ui->setupUi(this);
}

LevelDataDialog::~LevelDataDialog()
{
    delete ui;
}

bool LevelDataDialog::editLevelData()
{
    double gravity = PhysicsManager::getInstance().getGravity();
    int width = PhysicsManager::getInstance().getBoundingRect().width();
    int height = PhysicsManager::getInstance().getBoundingRect().height();

    ui->sb_gravity->setValue(gravity);
    ui->sb_width->setValue(width);
    ui->sb_height->setValue(height);

    if (this->exec())
    {
        PhysicsManager::getInstance().setGravity(ui->sb_gravity->value());
        PhysicsManager::getInstance().setWorldBounds(QRectF(0, 0, ui->sb_width->value(), ui->sb_height->value()));
        return true;
    }
    else
    {
        return false;
    }
}
