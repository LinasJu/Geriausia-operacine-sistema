#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include <cstdint>
#include "memoryblock.h"


class VirtualMachine
{
public:
    VirtualMachine(MemoryBlock* memory);

    void add();
    void sub();
    void mul();
    void div();

    void cmp();

    void ld(uint16_t address);
    void pt(uint16_t address);
    void ps(uint16_t value);

    void jp(uint16_t address);
    void je(uint16_t address);
    void jn(uint16_t address);
    void jl(uint16_t address);
    void jg(uint16_t address);

    void setc();
    void loop(uint16_t address);

    void fork();
    void isp();
    void isc();

    void halt();
    void stop();

    void prts();
    void prtn();
private:
    uint16_t* PC;
    uint16_t* SP;
    uint8_t* PID;
    uint8_t* CX;

    MemoryBlock* memory;
};

#endif // VIRTUALMACHINE_H
