#include "levelmanager.h"
#include "ui_levelmanager.h"
#include "gamecore.h"
#include "filemanager.h"
#include <QtGui>

LevelManager::LevelManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LevelManager)
{
    ui->setupUi(this);

    connect(ui->pb_add, SIGNAL(clicked()),
            this, SLOT(addLevel()));
    connect(ui->pb_remove, SIGNAL(clicked()),
            this, SLOT(removeSelectedLevel()));
    connect(ui->pb_rename, SIGNAL(clicked()),
            this, SLOT(renameSelectedLevel()));
    connect(ui->pb_setAsStart, SIGNAL(clicked()),
            this, SLOT(setSelectedAsStartLevel()));
    connect(ui->pb_edit, SIGNAL(clicked()),
            this, SLOT(editSelected()));
    connect(ui->pb_reloadCurrentLevel, SIGNAL(clicked()),
            this, SLOT(reloadCurrentLevel()));
    connect(ui->pb_saveCurrentLevel, SIGNAL(clicked()),
            this, SLOT(saveCurrentLevel()));
    connect(ui->sb_gravity, SIGNAL(valueChanged(double)),
            this, SLOT(changeLevelGravity()));
    connect(ui->sb_width, SIGNAL(valueChanged(int)),
            this, SLOT(changeLevelSize()));
    connect(ui->sb_height, SIGNAL(valueChanged(int)),
            this, SLOT(changeLevelSize()));
    connect(ui->cb_resolution, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeGameResolution()));
    connect(ui->pb_selectColor, SIGNAL(clicked()),
            this, SLOT(changeLevelColor()));

    connect(&GameCore::getInstance(), SIGNAL(levelDataChanged()),
            this, SLOT(update()));

    ui->cb_resolution->addItem("640x480");
    ui->cb_resolution->addItem("800x600");
    ui->cb_resolution->addItem("1024x768");

    ui->cb_resolution->setCurrentIndex(1);
}

LevelManager::~LevelManager()
{
    delete ui;
}

void LevelManager::update()
{
    //setup levels
    ui->lw_Levels->clear();
    QStringList levels = FileManager::getInstance().getAvailableLevels();
    foreach (QString level, levels)
    {
        QFont startLevelFont;
        startLevelFont.setItalic(true);
        QFont currentLevelFont;
        currentLevelFont.setBold(true);
        QFont startAndCurrentLevelFont;
        startAndCurrentLevelFont.setItalic(true);
        startAndCurrentLevelFont.setBold(true);

        bool startLevel = false;
        bool currentLevel = false;
        if (level == GameCore::getInstance().getCurrentGameStartLevel())
            startLevel = true;
        if (level == GameCore::getInstance().getCurrentLevel())
            currentLevel = true;

        level.chop(FileManager::getLevelExtensions().at(0).mid(1).size());

        QListWidgetItem *item = new QListWidgetItem(level);

        if (startLevel && currentLevel)
            item->setFont(startAndCurrentLevelFont);
        else if (startLevel)
            item->setFont(startLevelFont);
        else if (currentLevel)
            item->setFont(currentLevelFont);

        ui->lw_Levels->addItem(item);
    }

    //setup resolution
    QSize resolution = GameCore::getInstance().getResolution();
    if (resolution.width() == 640)
    {
        ui->cb_resolution->setCurrentIndex(0);
    }
    else if (resolution.width() == 800)
    {
        ui->cb_resolution->setCurrentIndex(1);
    }
    else if (resolution.height() == 1024)
    {
        ui->cb_resolution->setCurrentIndex(2);
    }
    else
    {
        ui->cb_resolution->setCurrentIndex(1);
    }

    //setup level details
    ui->sb_gravity->setValue(PhysicsManager::getInstance().getGravity());
    ui->sb_width->setValue(PhysicsManager::getInstance().getBoundingRect().width());
    ui->sb_height->setValue(PhysicsManager::getInstance().getBoundingRect().height());
}

void LevelManager::renameSelectedLevel()
{
    if (!ui->lw_Levels->currentItem())
        return;

    QInputDialog dlg(this);
    dlg.setWindowTitle("Enter New Name");
    dlg.setInputMode(QInputDialog::TextInput);
    dlg.setTextValue(ui->lw_Levels->currentItem()->text());
    dlg.setLabelText("Level Name:");

    if (dlg.exec() && !dlg.textValue().isEmpty() && ui->lw_Levels->currentItem()->text() + FileManager::getLevelExtensions().at(0).mid(1) != GameCore::getInstance().getCurrentLevel())
    {
        if (QFile::rename(FileManager::getInstance().getLevelPath() + ui->lw_Levels->currentItem()->text() + FileManager::getLevelExtensions().at(0).mid(1),
                          FileManager::getInstance().getLevelPath() + dlg.textValue() + FileManager::getLevelExtensions().at(0).mid(1)))
        {
            if (ui->lw_Levels->currentItem()->text() + FileManager::getLevelExtensions().at(0).mid(1) == GameCore::getInstance().getCurrentGameStartLevel())
            {
                GameCore::getInstance().setCurrentGameStartLevel(dlg.textValue() + FileManager::getLevelExtensions().at(0).mid(1));
            }
        }
    }

    this->update();
}

void LevelManager::addLevel()
{
    QInputDialog dlg(this);
    dlg.setWindowTitle("Enter the Name of the New Level");
    dlg.setInputMode(QInputDialog::TextInput);
    dlg.setLabelText("Level Name:");

    if (dlg.exec() && !dlg.textValue().isEmpty())
    {
        FileManager::getInstance().createNewLevel(dlg.textValue() + FileManager::getLevelExtensions().at(0).mid(1));
    }

    this->update();
}

void LevelManager::removeSelectedLevel()
{
    if (!ui->lw_Levels->currentItem()
            || ui->lw_Levels->count() < 2
            || ui->lw_Levels->currentItem()->text() + FileManager::getLevelExtensions().at(0).mid(1) == GameCore::getInstance().getCurrentLevel())
        return;

    QString fileName = ui->lw_Levels->currentItem()->text() + FileManager::getLevelExtensions().at(0).mid(1);

    FileManager::getInstance().removeLevel(fileName);

    this->update();

    if (GameCore::getInstance().getCurrentGameStartLevel() == fileName)
    {
        ui->lw_Levels->setCurrentItem(ui->lw_Levels->itemAt(0, 0));
        setSelectedAsStartLevel();
    }
}

void LevelManager::setSelectedAsStartLevel()
{
    if (!ui->lw_Levels->currentItem())
        return;

    GameCore::getInstance().setCurrentGameStartLevel(ui->lw_Levels->currentItem()->text() + FileManager::getLevelExtensions().at(0).mid(1));
    this->update();
}

void LevelManager::editSelected()
{
    if (!ui->lw_Levels->currentItem())
        return;

    //TODO: make it so that this only happens when a modification occurs (this would require modification tracking of some sort)
    QMessageBox saveMessage(this);
    saveMessage.setText("Would you like to save the current level before switching levels?\n(Unsaved changes will be permanently lost.)");
    saveMessage.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    int ret = saveMessage.exec();

    if (ret == QMessageBox::Save)
    {
        GameCore::getInstance().saveCurrentLevel();
        GameCore::getInstance().issueCommand(GameCore::ChangeLevel, ui->lw_Levels->currentItem()->text() + FileManager::getLevelExtensions().at(0).mid(1));
        this->update();
    }
    else if (ret == QMessageBox::Discard)
    {
        GameCore::getInstance().issueCommand(GameCore::ChangeLevel, ui->lw_Levels->currentItem()->text() + FileManager::getLevelExtensions().at(0).mid(1));
        this->update();
    }
    else if (ret == QMessageBox::Cancel)
    {

    }
}

void LevelManager::reloadCurrentLevel()
{
    QMessageBox saveMessage(this);
    saveMessage.setText("If you reload the level, all unsaved changes will be lost.  Continue?");
    saveMessage.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int ret = saveMessage.exec();

    if (ret == QMessageBox::Yes)
    {
        GameCore::getInstance().issueCommand(GameCore::ChangeLevel, GameCore::getInstance().getCurrentLevel());
        this->update();
    }
    else if (ret == QMessageBox::No)
    {
    }
}

void LevelManager::saveCurrentLevel()
{
    GameCore::getInstance().saveCurrentLevel();
    this->update();
}

void LevelManager::changeGameResolution()
{
    int width;
    int height;
    if (ui->cb_resolution->currentText() == "640x480")
    {
        width = 640;
        height = 480;
    }
    else if (ui->cb_resolution->currentText() == "800x600")
    {
        width = 800;
        height = 600;
    }
    else if (ui->cb_resolution->currentText() == "1024x768")
    {
        width = 1024;
        height = 768;
    }
    else
    {
        width = 800;
        height = 600;
    }

    GameCore::getInstance().setResolution(width, height);

    GameCore::getInstance().saveCurrentGame();
}

void LevelManager::changeLevelSize()
{
    PhysicsManager::getInstance().setWorldBounds(QRectF(0, 0, ui->sb_width->value(), ui->sb_height->value()));
}

void LevelManager::changeLevelGravity()
{
    PhysicsManager::getInstance().setGravity(ui->sb_gravity->value());
}

void LevelManager::changeLevelColor()
{
    QColorDialog dlg;

    dlg.setCurrentColor(GameCore::getInstance().getGraphicsScene()->getBackgroundColor());

    if (dlg.exec())
    {
        GameCore::getInstance().getGraphicsScene()->setBackgroundColor(dlg.currentColor());
    }
}
