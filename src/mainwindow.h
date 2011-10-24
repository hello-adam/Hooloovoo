#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "graphicsscene.h"
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

    GraphicsScene* getGraphicsScene() {return m_scene;}
    GraphicsView* getGraphicsView() {return m_view;}

protected:
    void keyPressEvent(QKeyEvent *ke);

private:
    Ui::MainWindow *ui;
    GraphicsScene *m_scene;
    GraphicsView *m_view;

private slots:
    void saveGame();
    void loadGame();
    void setSceneSize();
    void addObject();
};

#endif // MAINWINDOW_H
