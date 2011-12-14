#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graphicsview.h"
#include "levelmanager.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    Ui::MainWindow *ui;

    void initializeMenus();

    GraphicsView *m_gameGraphicsView;
    GraphicsView *m_editorGraphicsView;

    QMenu* m_playGameMenu;
    QMenu* m_createGameMenu;
    QMenu* m_levelMenu;
    QMenu* m_objectMenu;
    QMenu* m_helpMenu;

    LevelManager *m_levelManager;

signals:

public slots:
    void refreshGameList();

private slots:
    void startScreenEditGame();
    void startScreenPlayGame();
    void startScreenFileSelected();

    void switchToStartScreen();
    void switchToGameScreen();
    void switchToGameEditorScreen();

    void launchLevelManager();

    void launchAboutDialog();
};

#endif // MAINWINDOW_H
