#ifndef VM_H
#define VM_H

#include "memoryblock.h"
#include "vm.h"
#include "Cpu.h"

class VM
{
    VM(MemoryBlock* memory, Cpu* cpu);

    void next();

    void run();

    uint32_t get_from_stack(uint8_t address);
    void set_to_stack(uint8_t address, uint32_t value);

    uint32_t get_from_memory(uint8_t address);
    void set_to_memory(uint8_t address, uint32_t value);

    void add();
    void sub();
    void mul();
    void div();

    void cmp();

    void ld(uint8_t address);
    void pt(uint8_t address);
    void ps(uint8_t value);

    void jp(uint8_t address);
    void je(uint8_t address);
    void jn(uint8_t address);
    void jl(uint8_t address);
    void jg(uint8_t address);

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

    Cpu* cpu;

    MemoryBlock* memory;
};

#endif // VM_H
