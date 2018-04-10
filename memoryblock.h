#ifndef MEMORYBLOCK_H
#define MEMORYBLOCK_H

#include <cstdint>

class MemoryBlock
{
public:
    MemoryBlock();
    uint32_t get(uint8_t address);
    void set(uint8_t address, uint32_t value);
private:
    uint32_t cell[255];
};

#endif // MEMORYBLOCK_H
