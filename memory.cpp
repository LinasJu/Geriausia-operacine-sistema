#include "memory.h"

#include <iostream>
#include <QTextStream>

Memory::Memory()
{
    for(int i = 0 ; i < 256 ; i++){
        memory[i] = new MemoryBlock;
    }
}

QTextStream& qStdOut()
{
    static QTextStream ts( stdout );
    return ts;
}

uint32_t Memory::get(uint16_t address)
{
    uint8_t block = (address & 0x0FF0)>>4;
    uint8_t cell = address & 0xF;

    return memory[block]->get(cell);
}

void Memory::set(uint16_t address, uint32_t value)
{
    uint8_t block = (address & 0x0FF0)>>4;
    uint8_t cell = address & 0xF;

    memory[block]->set(cell, value);
}
MemoryBlock Memory::getMemoryBlock(uint8_t block){
    return *memory[block];
}

MemoryBlock *Memory::getMemoryBlockAddress(uint8_t block)
{
    return memory[block];
}
void Memory::setMemoryBlock(MemoryBlock *mem, int block){
    this->memory[block]=mem;
}
