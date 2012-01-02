#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGLWidget>
#include "gamecore.h"
#include "filemanager.h"
#include "gamefiledialog.h"
#include "propertydelegate.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setCentralWidget(ui->stackedWidget);

    m_gameGraphicsView = new GraphicsView();
    m_editorGraphicsView = new GraphicsView();

    m_gameGraphicsView->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    m_gameGraphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    m_gameGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_gameGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_editorGraphicsView->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    m_editorGraphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    ui->editorLayout->insertWidget(0, m_editorGraphicsView);
    ui->playLayout->insertWidget(0, m_gameGraphicsView);

//    connect(ui->tb_pause, SIGNAL(toggled(bool)),
//            GameCore::getInstance().getTogglePauseAction(), SLOT(setChecked(bool)));

    m_previewControlBar = this->addToolBar("Preview Controls:");
    m_previewControlBar->addAction(GameCore::getInstance().getTogglePauseAction());
    m_previewControlBar->setVisible(false);

    connect(ui->pb_createGame, SIGNAL(clicked()),
            GameCore::getInstance().getCreateGameAction(), SLOT(trigger()));
    connect(ui->pb_editGame, SIGNAL(clicked()),
            this, SLOT(startScreenEditGame()));
    connect(ui->pb_playGame, SIGNAL(clicked()),
            this, SLOT(startScreenPlayGame()));
    connect(ui->listWidget, SIGNAL(itemSelectionChanged()),
            this, SLOT(startScreenFileSelected()));

    connect(&GameCore::getInstance(), SIGNAL(hasSelectedObject(bool)),
            this, SLOT(gameObjectSelectionChanged(bool)));

    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);


    m_levelManagerDock = new QDockWidget("Level Manager", this);
    m_levelManager = new LevelManager(m_levelManagerDock);
    m_levelManagerDock->setWidget(m_levelManager);

    m_gameObjectEditorDock = new QDockWidget("Object Properties", this);
    m_gameObjectEditorView = new GameObjectEditWidget(m_gameObjectEditorDock);
    m_gameObjectEditorDock->setWidget(m_gameObjectEditorView);

    m_causeAndEffectEditDock = new QDockWidget("Object Cause And Effect", this);
    m_causeAndEffectEditWidget = new CauseAndEffectEditWidget(m_causeAndEffectEditDock);
    m_causeAndEffectEditDock->setWidget(m_causeAndEffectEditWidget);

    m_fileBrowserDock = new QDockWidget("Files", this);
    m_fileBrowserWidget = new GameObjectFileWidget(m_fileBrowserDock);
    m_fileBrowserDock->setWidget(m_fileBrowserWidget);

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

    delete m_levelManager;
    delete m_levelManagerDock;

    delete m_gameObjectEditorView;
    delete m_gameObjectEditorDock;

    delete m_causeAndEffectEditWidget;
    delete m_causeAndEffectEditDock;

    delete m_fileBrowserDock;

    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *ke)
{
    GameCore::getInstance().handleKeyEvent(ke);
}

void MainWindow::keyReleaseEvent(QKeyEvent *ke)
{
    GameCore::getInstance().handleKeyEvent(ke);
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

    m_levelMenu->addAction("&Manage Levels", this, SLOT(launchLevelManager()));
    m_levelMenu->addAction(GameCore::getInstance().getSaveLevelAction());
    m_levelMenu->addSeparator();
    m_levelMenu->addAction(GameCore::getInstance().getEditCurrentLevelAction());
    m_levelMenu->addAction(GameCore::getInstance().getAddObjectToLevelAction());

    m_objectMenu->addAction(GameCore::getInstance().getAddObjectToLevelAction());
    m_objectMenu->addAction(GameCore::getInstance().getEditSelectedObjectAction());
    m_objectMenu->addAction(GameCore::getInstance().getSaveSelectedObjectAction());
    m_objectMenu->addAction(GameCore::getInstance().getRemoveSelectedObjectAction());
    GameCore::getInstance().getCopySelectedObjectAction()->setShortcut(QKeySequence::Copy);
    m_objectMenu->addAction(GameCore::getInstance().getCopySelectedObjectAction());
    GameCore::getInstance().getPasteSelectedObjectAction()->setShortcut(QKeySequence::Paste);
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
        {
            m_editorGraphicsView->setScene(GameCore::getInstance().getGraphicsScene());
            m_gameGraphicsView->setScene(0);

            switchToGameEditorScreen();
            GameCore::getInstance().getTogglePauseAction()->setChecked(true);
        }
    }
}

void MainWindow::startScreenPlayGame()
{
    if (ui->listWidget->currentItem())
    {
        if (GameCore::getInstance().loadGame(ui->listWidget->currentItem()->text()))
        {
            m_gameGraphicsView->setScene(GameCore::getInstance().getGraphicsScene());
            m_editorGraphicsView->setScene(0);

            m_gameGraphicsView->setResolution(GameCore::getInstance().getResolution());

            switchToGameScreen();
            GameCore::getInstance().getTogglePauseAction()->setChecked(false);
        }
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

    m_gameObjectEditorDock->setVisible(false);
    m_causeAndEffectEditDock->setVisible(false);
    m_levelManagerDock->setVisible(false);
    m_previewControlBar->setVisible(false);
    m_fileBrowserDock->setVisible(false);

    QMenuBar* startScreenMenuBar = new QMenuBar();
    startScreenMenuBar->addMenu(m_helpMenu);
    this->setMenuBar(startScreenMenuBar);
}

void MainWindow::switchToGameScreen()
{
    ui->stackedWidget->setCurrentIndex(2);

    m_gameObjectEditorDock->setVisible(false);
    m_causeAndEffectEditDock->setVisible(false);
    m_levelManagerDock->setVisible(false);
    m_previewControlBar->setVisible(false);
    m_fileBrowserDock->setVisible(false);

    QMenuBar* playMenuBar = new QMenuBar();
    playMenuBar->addMenu(m_playGameMenu);
    playMenuBar->addMenu(m_helpMenu);
    this->setMenuBar(playMenuBar);
}

void MainWindow::switchToGameEditorScreen()
{
    ui->stackedWidget->setCurrentIndex(1);

    m_fileBrowserDock->setVisible(true);
    this->addDockWidget(Qt::BottomDockWidgetArea, m_fileBrowserDock, Qt::Horizontal);

    m_gameObjectEditorDock->setVisible(true);
    this->addDockWidget(Qt::RightDockWidgetArea, m_gameObjectEditorDock, Qt::Vertical);
    //m_gameObjectEditorDock->setMinimumWidth(300);

    m_causeAndEffectEditDock->setVisible(true);
    this->addDockWidget(Qt::RightDockWidgetArea, m_causeAndEffectEditDock, Qt::Vertical);
    //m_causeAndEffectEditDock->setMinimumWidth(300);

    this->tabifyDockWidget(m_gameObjectEditorDock, m_causeAndEffectEditDock);
    QList<QTabBar*> tabBars = this->findChildren<QTabBar*>();
    if(tabBars.count())
    {
        tabBars.first()->setCurrentIndex(0);
    }

    m_levelManagerDock->setVisible(true);
    this->addDockWidget(Qt::LeftDockWidgetArea, m_levelManagerDock, Qt::Vertical);
    //m_levelManagerDock->setMinimumWidth(150);

    m_previewControlBar->setVisible(true);

    QMenuBar* createMenuBar = new QMenuBar();
    createMenuBar->addMenu(m_createGameMenu);
    createMenuBar->addMenu(m_levelMenu);
    createMenuBar->addMenu(m_objectMenu);
    createMenuBar->addMenu(m_helpMenu);
    this->setMenuBar(createMenuBar);
}

void MainWindow::launchLevelManager()
{
//    m_levelManager->showNormal();
}

void MainWindow::gameObjectSelectionChanged(bool hasSelection)
{
    if (!hasSelection)
    {
        m_gameObjectEditorView->setGameObject(0);
        m_causeAndEffectEditWidget->setGameObject(0);
    }
    else
    {
        m_gameObjectEditorView->setGameObject(GameCore::getInstance().selectedObject());

        m_causeAndEffectEditWidget->setGameObject(GameCore::getInstance().selectedObject());
    }
}

void MainWindow::launchAboutDialog()
{

}
