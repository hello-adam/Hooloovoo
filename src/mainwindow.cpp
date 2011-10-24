#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtOpenGL/qgl.h>
#include <QFormLayout>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QDebug>

#include "graphicsscene.h"
#include "graphicsview.h"
#include "gamecore.h"
#include "gamefiledialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect (ui->action_Load, SIGNAL(triggered()), this, SLOT(loadGame()));
    connect (ui->action_Save, SIGNAL(triggered()), this, SLOT(saveGame()));

    connect(ui->action_Set_Size, SIGNAL(triggered()), this, SLOT(setSceneSize()));
    connect(ui->action_Add_Object, SIGNAL(triggered()), this, SLOT(addObject()));

    m_view = new GraphicsView(this);
    m_scene = new GraphicsScene();

    m_view->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    m_view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    m_view->setScene(m_scene);
    m_scene->setSceneRect(0, 0, 800, 600);

    this->setCentralWidget(m_view);
}

MainWindow::~MainWindow()
{
    delete ui;

    if (m_view)
            m_view->deleteLater();
}

void MainWindow::saveGame()
{
    GameFileDialog *dlg = new GameFileDialog(this);
    dlg->setAccept("Save");

    if (dlg->exec())
    {
        GameCore::getInstance()->saveGame(dlg->getFileName());
    }

    delete dlg;
}

void MainWindow::loadGame()
{
    GameFileDialog *dlg = new GameFileDialog(this);
    dlg->setAccept("Load");

    if (dlg->exec())
    {
        GameCore::getInstance()->loadGame(dlg->getFileName());
    }

    delete dlg;
}

void MainWindow::setSceneSize()
{
    GameCore::getInstance()->launchGlobalDataDialog();
}

void MainWindow::addObject()
{
    GameCore::getInstance()->launchAddObjectDialog();
}

void MainWindow::keyPressEvent(QKeyEvent *ke)
{
    GameCore::getInstance()->handleKeypress(ke);
}
