#include <QApplication>
#include "mainDialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainDialog d;
    d.show();
//    d.show();

    return a.exec();
}
