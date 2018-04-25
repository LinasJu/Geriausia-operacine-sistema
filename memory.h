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
    void setMemoryBlock(MemoryBlock *mem,int block);
    MemoryBlock getMemoryBlock(uint8_t block);
    MemoryBlock* getMemoryBlockAddress(uint8_t block);
private:
    MemoryBlock* memory[256];
};

#endif // MEMORY_H
