#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("KublaSoft");
    QCoreApplication::setApplicationName("LightTriggerQt");
    MainWindow w;
    w.show();
    return a.exec();
}
