#include <QtGui/QApplication>

#include "mainwindow.h"
#include "gamecore.h"

int main (int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    GameCore *core = GameCore::getInstance();

    core->initializeGameDirectoriesAndData();
    core->setScene(window.getGraphicsScene());

    window.show();

    return app.exec();
}
