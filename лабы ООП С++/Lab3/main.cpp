#include "mainwindow.h"

#include <QApplication>
#include "test.h"

int main(int argc, char *argv[])
{
    testClasses();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
