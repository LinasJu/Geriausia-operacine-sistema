#include "memoryblock.h"
#include <stdlib.h>

MemoryBlock::MemoryBlock()
{
    for(int i = 0; i < 256; i++){
        cell[i] = rand() % 0x10000;
    }
}

uint32_t MemoryBlock::get(uint8_t address)
{
    return cell[address];
}

void MemoryBlock::set(uint8_t address, uint32_t value)
{
    cell[address] = value;
}
