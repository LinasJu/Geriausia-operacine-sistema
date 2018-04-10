#include "memory.h"

#include <iostream>
#include <QTextStream>

Memory::Memory()
{
    for(int i = 0 ; i < 16 ; i++){
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
    uint8_t block = (address & 0xFF00) >> 8;
    uint8_t cell = address & 0xFF;

    return memory[block]->get(cell);
}

void Memory::set(uint16_t address, uint32_t value)
{
    uint8_t block = (address & 0xFF00) >> 8;
    uint8_t cell = address & 0xFF;

    memory[block]->set(cell, value);
}
