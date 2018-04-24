#include "vm.h"

VM::VM(MemoryBlock memory[16])
{
    this->memory = memory;
}

void VM::add()
{
    uint32_t a = memory->get(*SP - 1);
    uint32_t b = memory->get(*SP);
    uint64_t result = a + b;

    if(!result){
        *SF += 1;
    }

    if(result & 0x80000000){
        *SF += 2;
    }

    if(result > 0xFFFFFFFF){
        *SF += 4;
    }

    if( (*SF & 0x02) && (a < 0x7FFFFFFF) && (b < 0x7FFFFFFF) ){
        *SF += 8;
    }
    if( !(*SF & 0x02) && (a > 0x7FFFFFFF) && (b > 0x7FFFFFFF) ){
        *SF += 8;
    }

    memory->set(*SP - 1, memory->get(*SP - 1) + memory->get(*SP));
    *SP--;
}

void VM::sub()
{
    memory->set(*SP - 1, memory->get(*SP - 1) - memory->get(*SP));
    *SP--;
}

void VM::mul()
{
    memory->set(*SP - 1, memory->get(*SP - 1) * memory->get(*SP));
    *SP--;
}

void VM::div()
{
    memory->set(*SP - 1, memory->get(*SP - 1) / memory->get(*SP));
    *SP--;
}

void VM::cmp()
{

}

void VM::ld(uint16_t address)
{

}

void VM::pt(uint16_t address)
{

}

void VM::ps(uint16_t value)
{

}

void VM::jp(uint16_t address)
{

}

void VM::je(uint16_t address)
{

}

void VM::jn(uint16_t address)
{

}

void VM::jl(uint16_t address)
{

}

void VM::jg(uint16_t address)
{

}

void VM::setc()
{

}

void VM::loop(uint16_t address)
{

}

void VM::fork()
{

}

void VM::isp()
{

}

void VM::isc()
{

}

void VM::halt()
{

}

void VM::stop()
{

}

void VM::prts()
{

}

void VM::prtn()
{

}
