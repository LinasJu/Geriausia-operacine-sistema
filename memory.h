#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include "memoryblock.h"

class Memory
{
public:
    Memory();
    uint32_t get(uint16_t address);
    void set(uint16_t address, uint32_t value);
private:
    MemoryBlock* memory[15];
};

#endif // MEMORY_H
