#include "mainwindow.h"
#include <QApplication>
#include "dijkstra.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    test_dijkstra();

    return a.exec();
}
