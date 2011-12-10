#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGLWidget>
#include "gamecore.h"
#include "filemanager.h"
#include "gamefiledialog.h"

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
    ui->editorLayout->insertWidget(0, m_gameGraphicsView);

    connect(ui->tb_pause, SIGNAL(toggled(bool)),
            GameCore::getInstance().getTogglePauseAction(), SLOT(setChecked(bool)));

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

    GameCore::getInstance().setDialogParent(this);
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

    m_playGameMenu->addAction(GameCore::getInstance().getSavePlayStateAction());
    m_playGameMenu->addAction(GameCore::getInstance().getLoadPlayStateAction());
    m_playGameMenu->addSeparator();
    m_playGameMenu->addAction(GameCore::getInstance().getSwitchGameAction());
    m_playGameMenu->addSeparator();
    m_playGameMenu->addAction("&Exit to Start Screen", this, SLOT(switchToStartScreen()));
    m_playGameMenu->addAction("&Exit Program", this, SLOT(close()));

    m_createGameMenu->addAction(GameCore::getInstance().getCreateGameAction());
    m_createGameMenu->addAction(GameCore::getInstance().getSwitchGameAction());
    m_createGameMenu->addSeparator();
    m_createGameMenu->addAction("&Exit to Start Screen", this, SLOT(switchToStartScreen()));
    m_createGameMenu->addAction("&Exit Program", this, SLOT(close()));

    m_levelMenu->addAction(GameCore::getInstance().getManageLevelsAction());
    m_levelMenu->addAction(GameCore::getInstance().getSaveLevelAction());
    m_levelMenu->addSeparator();
    m_levelMenu->addAction(GameCore::getInstance().getEditCurrentLevelAction());
    m_levelMenu->addAction(GameCore::getInstance().getAddObjectToLevelAction());

    QAction *action;
    m_objectMenu->addAction(GameCore::getInstance().getAddObjectToLevelAction());
    m_objectMenu->addAction(GameCore::getInstance().getEditSelectedObjectAction());
    m_objectMenu->addAction(GameCore::getInstance().getSaveSelectedObjectAction());
    m_objectMenu->addAction(GameCore::getInstance().getRemoveSelectedObjectAction());
    m_objectMenu->addAction(GameCore::getInstance().getCopySelectedObjectAction());
    m_objectMenu->addAction(GameCore::getInstance().getPasteSelectedObjectAction());

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
        if (GameCore::getInstance().loadGame(ui->listWidget->currentItem()->text()))
            switchToGameEditorScreen();
    }
}

void MainWindow::startScreenPlayGame()
{
    if (ui->listWidget->currentItem())
    {
        if (GameCore::getInstance().loadGame(ui->listWidget->currentItem()->text()))
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

void MainWindow::launchAboutDialog()
{

}
