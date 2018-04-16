#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{

    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void addToTable(uint8_t row, uint8_t column, uint32_t item);

private:
    Ui::MainWindow *ui;
    void addToolTip(uint8_t row,uint8_t column, uint32_t item);
    void headerToolTip(uint8_t row);
    void initTable();
    void initRealMemoryTable();
};

#endif // MAINWINDOW_H
