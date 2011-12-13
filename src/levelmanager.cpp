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

    connect(&GameCore::getInstance(), SIGNAL(levelDataChanged()),
            this, SLOT(update()));
}

LevelManager::~LevelManager()
{
    delete ui;
}

void LevelManager::update()
{
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

//    if (GameCore::getInstance().getCurrentLevel() == ui->lw_Levels->currentItem()->text() + FileManager::getLevelExtensions().at(0).mid(1))
//        return;

    GameCore::getInstance().issueCommand(GameCore::ChangeLevel, ui->lw_Levels->currentItem()->text() + FileManager::getLevelExtensions().at(0).mid(1));
    this->update();
}
