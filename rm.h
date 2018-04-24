#ifndef RM_H
#define RM_H
#include "memory.h"
//#include "virtualmachine.h"
#include <cstdint>
#include <ctime>
#include <cstdlib>
#include "cpu.h"


class RM
{
public:
    RM();
    Memory *mem;
    void addVM();
    bool newTable();
    Cpu *cp;
private:
    bool isTaken(uint32_t block, uint8_t curr);
    void initPageTable();
    void inputInterrupt();
    void outputInterrupt();
    void timerInterrupt();
    void supervisorInterrupt();
    void programInterrupt();


};

#endif // RM_H
