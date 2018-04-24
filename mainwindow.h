#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include "rm.h"
#include <QFileDialog>
namespace Ui {
class MainWindow;
}
class RM;
class MainWindow : public QMainWindow
{

    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void addToTable(uint8_t row, uint8_t column, uint32_t item);
    void initTable();
    void appendOutput(std::string str);
    std::string intToHexStr(int a,int pos);
    void update();
    void updateRealTable();
    friend class RM;
private slots:
     void on_pushButton_3_clicked();

     void on_pushButton_clicked();

     void on_pushButton_2_clicked();

private:
     Ui::MainWindow *ui;
     RM* realmachine;
    void addToolTip(uint8_t row,uint8_t column, uint32_t item);
    void headerToolTip(uint8_t row);
    void initRealMemoryTable();
};

#endif // MAINWINDOW_H
