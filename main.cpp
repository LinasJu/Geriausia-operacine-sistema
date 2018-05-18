#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <iostream>
#include <cstdint>
#include <string>
#include <Windows.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *w= new MainWindow();
    w->show();
    return a.exec();
}
