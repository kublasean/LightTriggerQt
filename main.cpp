#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("KublaSoft");
    QCoreApplication::setApplicationName("LightTriggerQt");

    QPalette pal;
    pal.setColor(QPalette::Highlight, QColor("orange"));
    qApp->setPalette(pal);

    MainWindow w;
    w.show();
    return a.exec();
}
