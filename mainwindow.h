#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>
#include <QPixmap>
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

     void on_pushButton_clicked();

     void on_checkBox_clicked();

     void on_runButton_clicked();

     void on_connectButton_clicked();

     void on_stepButton_clicked();

     void on_disconnectButton_clicked();

private:
     Ui::MainWindow *ui;
     RM* realmachine;
    void addToolTip(uint8_t row,uint8_t column, uint32_t item);
    void headerToolTip(uint8_t row);
    void initRealMemoryTable();
    void changeRunButtonState(bool a);
    void changeStepButtonState(bool a);
};

#endif // MAINWINDOW_H
