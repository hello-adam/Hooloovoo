#include <QtGui/QApplication>

#include "mainwindow.h"

int main (int argc, char *argv[])
{
    QApplication app(argc, argv);

//    app.setStyleSheet("QTreeView {alternate-background-color: rgba(0,0,0,50);"
//                      "background-color: rgba(255,255,255,255); }"
//                      "QTreeView::item { border: 1px solid darkgray; }");

    app.setWindowIcon(QIcon(":/images/hooloovoo_icon_6464.png"));

    MainWindow window;

    window.showMaximized();

    return app.exec();
}
