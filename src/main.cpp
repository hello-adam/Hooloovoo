#include <QtGui/QApplication>

#include "mainwindow.h"

int main (int argc, char *argv[])
{
    QApplication app(argc, argv);

//    app.setStyleSheet("QTreeView {alternate-background-color: rgba(0,0,0,50);"
//                      "background-color: rgba(255,255,255,255); }"
//                      "QTreeView::item { border: 1px solid darkgray; }");

    MainWindow window;

    window.showMaximized();

    return app.exec();
}
