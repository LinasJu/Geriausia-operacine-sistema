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
#include "lightbulb.h"
#include "mainwindow.h"
#include "parsed_program.hs"
#include <QTableWidget>
class MainWindow;
class Printer;
class RM
{
public:
    RM();
    ~RM();
    friend class MainWindow;
    void addVM();
    bool newTable();
private:
    MainWindow *w = NULL;
    Printer *printer;
    FlashReader *flash;
    Lightbulb *lightbulb;
    VM *vms[15];
    Memory *mem;
    Cpu *cp;
    uint32_t *supervisorMemory = nullptr;
    uint16_t supervisorSize = 0;
    ParsedProgram *parseProgram();


    bool isTaken(uint32_t block, uint8_t curr);
    void initWindow(MainWindow &w);
    void initPageTable();
    void inputOutputInterrupt();
    void timerInterrupt();
    void supervisorInterrupt();
    void programInterrupt();
    void updateGUI();
    int getStackPosition();
    void insertFlash(std::string path);
    VM getNext();
    VM *current;
    void next();
    void run();
    bool test();
    void readFromFlash(char *destination, uint16_t bytes);
    void loadFlashToSupervisorMemory();

};

#endif // RM_H
