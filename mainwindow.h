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
    explicit MainWindow(uint8_t *pid,uint16_t *pc,uint16_t *sp, uint8_t *cx,QWidget *parent = 0);
    ~MainWindow();
    uint8_t *pid,*cx;
    uint16_t *sp, *pc;
    void addToTable(uint8_t row, uint8_t column, uint32_t item);
    void update();
    void initTable();

private slots:
     void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    void addToolTip(uint8_t row,uint8_t column, uint32_t item);
    void headerToolTip(uint8_t row);

    void initRealMemoryTable();
};

#endif // MAINWINDOW_H
