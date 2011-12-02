#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graphicsview.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    void initializeMenus();

    GraphicsView *m_gameGraphicsView;

    QMenu* m_playGameMenu;
    QMenu* m_createGameMenu;
    QMenu* m_levelMenu;
    QMenu* m_objectMenu;
    QMenu* m_helpMenu;

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

    void createGame();
    void switchGame();

    void newLevel();
    void saveLevel();
    void loadLevel();
    void editLevelData();
    void addObject();

    void savePlayState();
    void loadPlayState();

    void saveSelectedObject();
    void editSelectedObject();
    void removeSelectedObject();
    void copySelectedObject();
    void pasteObjectFromClipboard();

    void launchAboutDialog();
};

#endif // MAINWINDOW_H
