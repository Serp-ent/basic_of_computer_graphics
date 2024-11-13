#include "mainwindow.h"

#include <Ekran.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Ekran e;
    e.show();
    return a.exec();
}
