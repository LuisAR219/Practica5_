#include "mainwindow.h"
#include <QApplication>
#include "simulacion_inicio.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    ejecutarSimulacion();
    return a.exec();
}
