#include "mainwindow.h"
#include "standardcalculator.h"
#include "scientificcalculator.h"
#include "graphingcalculator.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("Fusion");
    a.setWindowIcon(QIcon(":/icons/app.png"));


    MainWindow w;
    w.show();

    return QApplication::exec();
}
