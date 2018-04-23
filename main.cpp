#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <cstdint>
#include <string>
#include <Windows.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    uint8_t aa=2;
    uint8_t bb=2;
    uint16_t cc=4;
    uint16_t dd=4;
    MainWindow w(&aa,&dd,&cc,&bb);
    w.show();
    w.update();
    return a.exec();
}
