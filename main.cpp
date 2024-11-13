#include "mainwindow.h"
#include <QDebug>

#include <Ekran.h>
#include <QApplication>

int
main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  qDebug() << "Application starts";
  Ekran e;
  e.show();
  return a.exec();
}
