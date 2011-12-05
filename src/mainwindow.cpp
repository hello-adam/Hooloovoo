#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGLWidget>
#include "gamecore.h"
#include "filemanager.h"
#include "gamefiledialog.h"
#include "leveldatadialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_gameGraphicsView = new GraphicsView();

    m_gameGraphicsView->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    m_gameGraphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    m_gameGraphicsView->setScene(GameCore::getInstance().getGraphicsScene());

    this->setCentralWidget(ui->stackedWidget);
    ui->editorLayout->addWidget(m_gameGraphicsView);

    connect(ui->pb_createGame, SIGNAL(clicked()),
            this, SLOT(createGame()));
    connect(ui->pb_editGame, SIGNAL(clicked()),
            this, SLOT(startScreenEditGame()));
    connect(ui->pb_playGame, SIGNAL(clicked()),
            this, SLOT(startScreenPlayGame()));
    connect(ui->listWidget, SIGNAL(itemSelectionChanged()),
            this, SLOT(startScreenFileSelected()));

    initializeMenus();

    this->refreshGameList();

    switchToStartScreen();
}

MainWindow::~MainWindow()
{
    if (m_playGameMenu)
        delete m_playGameMenu;
    if (m_createGameMenu)
        delete m_createGameMenu;
    if (m_levelMenu)
        delete m_levelMenu;
    if (m_helpMenu)
        delete m_helpMenu;

    delete m_gameGraphicsView;

    delete ui;
}

void MainWindow::initializeMenus()
{
    m_playGameMenu = new QMenu("&Game");
    m_createGameMenu = new QMenu("&Game");
    m_levelMenu = new QMenu("&Level");
    m_helpMenu = new QMenu("&Help");
    m_objectMenu = new QMenu("&Object");

    m_playGameMenu->addAction("&Save Game", this, SLOT(savePlayState()));
    m_playGameMenu->addAction("&Load Game", this, SLOT(loadPlayState()));
    m_playGameMenu->addSeparator();
    m_playGameMenu->addAction("&Switch Game", this, SLOT(switchGame()));
    m_playGameMenu->addSeparator();
    m_playGameMenu->addAction("&Exit to Start Screen", this, SLOT(switchToStartScreen()));
    m_playGameMenu->addAction("&Exit Program", this, SLOT(close()));

    m_createGameMenu->addAction("&New Game Project", this, SLOT(createGame()));
    m_createGameMenu->addAction("&Switch Game Project", this, SLOT(switchGame()));
    m_createGameMenu->addSeparator();
    m_createGameMenu->addAction("&Exit to Start Screen", this, SLOT(switchToStartScreen()));
    m_createGameMenu->addAction("&Exit Program", this, SLOT(close()));

    m_levelMenu->addAction("&New Level", this, SLOT(newLevel()));
    m_levelMenu->addAction("&Save Level", this, SLOT(saveLevel()));
    m_levelMenu->addAction("&Switch Level", this, SLOT(loadLevel()));
    m_levelMenu->addSeparator();
    m_levelMenu->addAction("&Edit Level Properties", this, SLOT(editLevelData()));
    m_levelMenu->addAction("&Add Object To Level", this, SLOT(addObject()));

    QAction *action;
    m_objectMenu->addAction("&Add Object To Level", this, SLOT(addObject()));
    action = m_objectMenu->addAction("&Edit Selected Object", this, SLOT(editSelectedObject()));
    connect(&GameCore::getInstance(), SIGNAL(hasSelectedObject(bool)), action, SLOT(setEnabled(bool)));
    action = m_objectMenu->addAction("&Save Selected Object", this, SLOT(saveSelectedObject()));
    connect(&GameCore::getInstance(), SIGNAL(hasSelectedObject(bool)), action, SLOT(setEnabled(bool)));
    action = m_objectMenu->addAction("&Remove Selected Object", this, SLOT(removeSelectedObject()));
    connect(&GameCore::getInstance(), SIGNAL(hasSelectedObject(bool)), action, SLOT(setEnabled(bool)));
    action = m_objectMenu->addAction("&Copy Selected Object", this, SLOT(copySelectedObject()));
    connect(&GameCore::getInstance(), SIGNAL(hasSelectedObject(bool)), action, SLOT(setEnabled(bool)));
    action = m_objectMenu->addAction("&Paste Copied Object", this, SLOT(pasteObjectFromClipboard()));
    connect(&GameCore::getInstance(), SIGNAL(hasObjectOnClipboard(bool)), action, SLOT(setEnabled(bool)));

    m_helpMenu->addAction("&About...", this, SLOT(launchAboutDialog()));
}

void MainWindow::refreshGameList()
{
    ui->listWidget->clear();
    ui->listWidget->addItems(FileManager::getInstance().getAvailableGames());
}

void MainWindow::startScreenEditGame()
{
    if (ui->listWidget->currentItem())
    {
        GameCore::getInstance().deserializeLevel(FileManager::getInstance().loadGame(ui->listWidget->currentItem()->text()));
        switchToGameEditorScreen();
    }
}

void MainWindow::startScreenPlayGame()
{
    if (ui->listWidget->currentItem())
    {
        GameCore::getInstance().deserializeLevel(FileManager::getInstance().loadGame(ui->listWidget->currentItem()->text()));
        switchToGameScreen();
    }
}

void MainWindow::startScreenFileSelected()
{
    bool enabled = false;
    if (ui->listWidget->currentItem())
    {
        enabled = true;
    }

    ui->pb_editGame->setEnabled(enabled);
    ui->pb_playGame->setEnabled(enabled);
}

void MainWindow::switchToStartScreen()
{
    ui->stackedWidget->setCurrentIndex(0);

    QMenuBar* startScreenMenuBar = new QMenuBar();
    startScreenMenuBar->addMenu(m_helpMenu);
    this->setMenuBar(startScreenMenuBar);
}

void MainWindow::switchToGameScreen()
{
    ui->stackedWidget->setCurrentIndex(2);

    QMenuBar* playMenuBar = new QMenuBar();
    playMenuBar->addMenu(m_playGameMenu);
    playMenuBar->addMenu(m_helpMenu);
    this->setMenuBar(playMenuBar);
}

void MainWindow::switchToGameEditorScreen()
{
    ui->stackedWidget->setCurrentIndex(1);

    QMenuBar* createMenuBar = new QMenuBar();
    createMenuBar->addMenu(m_createGameMenu);
    createMenuBar->addMenu(m_levelMenu);
    createMenuBar->addMenu(m_objectMenu);
    createMenuBar->addMenu(m_helpMenu);
    this->setMenuBar(createMenuBar);
}

void MainWindow::createGame()
{
    GameFileDialog dlg(this);

    dlg.setAcceptMode(GameFileDialog::Create);
    dlg.setFileType(GameFileDialog::Game);

    if (dlg.exec())
    {
        if (!dlg.getFileName().isEmpty())
        {
            GameCore::getInstance().deserializeLevel(FileManager::getInstance().createNewGame(dlg.getFileName()));
            switchToGameEditorScreen();
        }
    }
}

void MainWindow::switchGame()
{
    GameFileDialog dlg(this);

    dlg.setAcceptMode(GameFileDialog::Load);
    dlg.setFileType(GameFileDialog::Game);

    if (dlg.exec() && !dlg.getFileName().isEmpty())
    {
        GameCore::getInstance().deserializeLevel(FileManager::getInstance().loadGame(dlg.getFileName()));
    }
}

void MainWindow::newLevel()
{
    GameFileDialog dlg(this);

    dlg.setAcceptMode(GameFileDialog::Create);
    dlg.setFileType(GameFileDialog::Level);

    if (dlg.exec() && !dlg.getFileName().isEmpty())
    {
        GameCore::getInstance().deserializeLevel(FileManager::getInstance().createNewLevel(dlg.getFileName()));
    }
}

void MainWindow::saveLevel()
{
    GameFileDialog dlg(this);

    dlg.setAcceptMode(GameFileDialog::Save);
    dlg.setFileType(GameFileDialog::Level);

    if (dlg.exec() && !dlg.getFileName().isEmpty())
    {
        FileManager::getInstance().saveLevel(GameCore::getInstance().serializeLevel(), dlg.getFileName());
    }
}

void MainWindow::loadLevel()
{
    GameFileDialog dlg(this);

    dlg.setAcceptMode(GameFileDialog::Load);
    dlg.setFileType(GameFileDialog::Level);

    if (dlg.exec() && !dlg.getFileName().isEmpty())
    {
        GameCore::getInstance().deserializeLevel(FileManager::getInstance().loadLevel(dlg.getFileName()));
    }
}

void MainWindow::editLevelData()
{
    LevelDataDialog dlg;
    dlg.editLevelData();
}

void MainWindow::addObject()
{
    GameFileDialog dlg(this);

    dlg.setAcceptMode(GameFileDialog::Select);
    dlg.setFileType(GameFileDialog::GameObject);

    if (dlg.exec() && !dlg.getFileName().isEmpty())
    {
        QPointF pos = m_gameGraphicsView->mapToScene(m_gameGraphicsView->width()/2, m_gameGraphicsView->height()/2);
        GameCore::getInstance().addObjectToLevel(FileManager::getInstance().loadGameObject(dlg.getFileName()), pos);
    }
}

void MainWindow::savePlayState()
{

}

void MainWindow::loadPlayState()
{

}

void MainWindow::saveSelectedObject()
{
    GameFileDialog dlg(this);

    dlg.setAcceptMode(GameFileDialog::Save);
    dlg.setFileType(GameFileDialog::GameObject);

    if (dlg.exec() && !dlg.getFileName().isEmpty())
    {
        FileManager::getInstance().saveGameObject(GameCore::getInstance().serializeSelectedObject(), dlg.getFileName());
    }
}

void MainWindow::editSelectedObject()
{

}

void MainWindow::removeSelectedObject()
{

}

void MainWindow::copySelectedObject()
{
    GameCore::getInstance().copySelectedObjectToClipboard();
}

void MainWindow::pasteObjectFromClipboard()
{
    GameCore::getInstance().addObjectToLevel(GameCore::getInstance().getClipboardElement());
}

void MainWindow::launchAboutDialog()
{

}
