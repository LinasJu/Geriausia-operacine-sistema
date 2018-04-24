#ifndef RM_H
#define RM_H
#include "memory.h"
#include "vm.h"
#include <cstdint>
#include <ctime>
#include <cstdlib>
#include "cpu.h"
#include "printer.h"
#include "flashreader.h"
#include "mainwindow.h"
#include <QTableWidget>
class MainWindow;
class Printer;
class RM
{
public:
    RM();
    friend class MainWindow;
    void addVM();
    bool newTable();
private:
    MainWindow *w = NULL;
    Printer *printer;
    FlashReader *flash;
    VM *vms[15];
    Memory *mem;
    Cpu *cp;
    bool isTaken(uint32_t block, uint8_t curr);
    void initWindow(MainWindow &w);
    void initPageTable();
    void inputInterrupt();
    void outputInterrupt();
    void timerInterrupt();
    void supervisorInterrupt();
    void programInterrupt();
    void updateGUI();
    int getStackPosition();
    void insertFlash(std::string path);

};

#endif // RM_H
