#include "vm.h"

VM::VM(MemoryBlock memory[16], Cpu* cpu)
{
    this->memory = memory;
    this->cpu = cpu;
}

void VM::next()
{
    uint32_t cmd = get_from_memory(*PC);

    switch (cmd) {
    case ('A' << 24) + ('D' << 16) + ('D' << 8):
        add();
        *PC++;
        break;
    case ('S' << 24) + ('U' << 16) + ('B' << 8):
        sub();
        *PC++;
        break;
    case ('M' << 24) + ('U' << 16) + ('L' << 8):
        mul();
        *PC++;
        break;
    case ('D' << 24) + ('I' << 16) + ('V' << 8):
        div();
        *PC++;
        break;
    case ('C' << 24) + ('M' << 16) + ('P' << 8):
        cmp();
        *PC++;
        break;
    case ('S' << 24) + ('E' << 16) + ('T' << 8) + 'C':
        setc();
        *PC++;
        break;
    default:
        break;
    }
    switch (cmd & 0xFF00 >> 16) {
    case ('L' << 8) + 'D':
        ld((uint8_t)cmd&0x00FF);
        *PC++;
        break;
    case ('P' << 8) + 'T':
        pt((uint8_t)cmd&0x00FF);
        *PC++;
        break;
    case ('P' << 8) + 'S':
        ps((uint8_t)cmd&0x00FF);
        *PC++;
        break;
    case ('J' << 8) + 'P':
        jp((uint8_t)cmd&0x00FF);
        break;
    case ('J' << 8) + 'E':
        je((uint8_t)cmd&0x00FF);
        break;
    case ('J' << 8) + 'N':
        jn((uint8_t)cmd&0x00FF);
        *PC++;
        break;
    case ('J' << 8) + 'L':
        jl((uint8_t)cmd&0x00FF);
        break;
    case ('J' << 8) + 'G':
        jg((uint8_t)cmd&0x00FF);
        break;
    case ('L' << 8) + 'P':
        loop((uint8_t)cmd&0x00FF);
        break;
    default:
        break;
    }
}

void VM::run()
{
    while(*PC <= 111){
        next();
    }
}

uint32_t VM::get_from_stack(uint8_t address)
{
    uint8_t first = ((address & 0xF0) + 0xE0) >> 4;
    uint8_t second = address & 0x0F;
    return memory[first].get(second);
}

void VM::set_to_stack(uint8_t address, uint32_t value)
{
    uint8_t first = ((address & 0xF0) + 0xE0) >> 4;
    uint8_t second = address & 0x0F;
    memory[first].set(second, value);
}

uint32_t VM::get_from_memory(uint8_t address)
{
    uint8_t first = (address & 0xF0) >> 4;
    uint8_t second = address & 0x0F;
    return memory[first].get(second);
}

void VM::set_to_memory(uint8_t address, uint32_t value)
{
    uint8_t first = (address & 0xF0) >> 4;
    uint8_t second = address & 0x0F;
    memory[first].set(second, value);
}

void VM::add()
{
    uint32_t a = get_from_stack(*SP - 1);
    uint32_t b = get_from_stack(*SP);

    cpu->clearSF();

    uint64_t result = (uint64_t)a + (uint64_t)b;


    if(!(result&0xFFFFFFFF)){
        cpu->setZF(true);
    }

    if(result & 0x80000000){
        cpu->setSF(true);
    }

    if(result > 0xFFFFFFFF){
        cpu->setCF(true);
    }

    if( cpu->getSiF() && (a < 0x7FFFFFFF) && (b < 0x7FFFFFFF) ){
        cpu->setOF(true);
    }
    if( !cpu->getSiF() && (a > 0x7FFFFFFF) && (b > 0x7FFFFFFF) ){
        cpu->setOF(true);
    }

    set_to_stack(*SP - 1, (uint32_t)(result&0xFFFFFFFF));

    *SP--;
}

void VM::sub()
{
    uint32_t a = get_from_stack(*SP - 1);
    uint32_t b = get_from_stack(*SP);

    cpu->clearSF();

    uint64_t result = (uint64_t)a - (uint64_t)b;

    if(!(result&0xFFFFFFFF)){
        cpu->setZF(true);
    }

    if(result & 0x80000000){
        cpu->setSF(true);
    }

    if(result > 0xFFFFFFFF){
        cpu->setCF(true);
    }

    if( !cpu->getSiF() && (a < 0x7FFFFFFF) && (b > 0x7FFFFFFF) ){
        cpu->setOF(true);
    }
    if( cpu->getSiF() && (a > 0x7FFFFFFF) && (b < 0x7FFFFFFF) ){
        cpu->setOF(true);
    }

    set_to_stack(*SP - 1, (uint32_t)(result&0xFFFFFFFF));

    *SP--;
}

void VM::mul()
{
    uint32_t a = get_from_stack(*SP - 1);
    uint32_t b = get_from_stack(*SP);

    cpu->clearSF();

    uint64_t result = (uint64_t)a * (uint64_t)b;

    if(!(result&0xFFFFFFFF)){
        cpu->setZF(true);
    }

    if(result & 0x80000000){
        cpu->setSF(true);
    }

    if(result > 0xFFFFFFFF){
        cpu->setCF(true);
    }

    if( cpu->getSiF() && (a < 0x7FFFFFFF) && (b < 0x7FFFFFFF) ){
        cpu->setOF(true);
    }
    if( cpu->getSiF() && (a > 0x7FFFFFFF) && (b > 0x7FFFFFFF) ){
        cpu->setOF(true);
    }
    if( !cpu->getSiF() && ( ( (a > 0x7FFFFFFF) && (b < 0x7FFFFFFF) ) || ( (a < 0x7FFFFFFF) && (b > 0x7FFFFFFF) ) ) ){
        cpu->setOF(true);
    }

    set_to_stack(*SP - 1, (uint32_t)(result&0xFFFFFFFF));

    *SP--;
}

void VM::div()
{
    uint32_t a = get_from_stack(*SP - 1);
    uint32_t b = get_from_stack(*SP);

    uint64_t result = (uint64_t)a / (uint64_t)b;

    cpu->clearSF();

    if(!b){
        cpu->setOF(true);
        return;
    }

    if(!(result&0xFFFFFFFF)){
        cpu->setZF(true);
    }

    if(result & 0x80000000){
        cpu->setSF(true);
    }
    if(result > 0xFFFFFFFF){
        cpu->setCF(true);
    }

    set_to_stack(*SP - 1, (uint32_t)(result&0xFFFFFFFF));

    *SP--;
}

void VM::cmp()
{
    uint32_t a = get_from_stack(*SP - 1);
    uint32_t b = get_from_stack(*SP);

    cpu->clearSF();

    uint64_t result = (uint64_t)a - (uint64_t)b;

    if(!(result&0xFFFFFFFF)){
        cpu->setZF(true);
    }

    if(result & 0x80000000){
        cpu->setSF(true);
    }

    if(result > 0xFFFFFFFF){
        cpu->setCF(true);
    }

    if( !cpu->getSiF() && (a < 0x7FFFFFFF) && (b > 0x7FFFFFFF) ){
        cpu->setOF(true);
    }
    if( cpu->getSiF() && (a > 0x7FFFFFFF) && (b < 0x7FFFFFFF) ){
        cpu->setOF(true);
    }
}

void VM::ld(uint8_t address)
{
    uint32_t value = get_from_memory(address);
    set_to_stack(*SP+1, value);
    *SP++;
}

void VM::pt(uint8_t address)
{
    uint32_t value = get_from_stack(*SP);
    set_to_memory(address, value);
    *SP--;
}

void VM::ps(uint8_t value)
{
    set_to_stack(*SP+1, value);
    *SP++;
}

void VM::jp(uint8_t address)
{
    *PC = address;
}

void VM::je(uint8_t address)
{
    if(cpu->getZF()){
        *PC = address;
    }
}

void VM::jn(uint8_t address)
{
    if(!cpu->getZF()){
        *PC = address;
    }
}

void VM::jl(uint8_t address)
{
    if(cpu->getSiF() == !cpu->getOF()){
        *PC = address;
    }
}

void VM::jg(uint8_t address)
{
    if(cpu->getSiF() == cpu->getOF()){
        *PC = address;
    }
}

void VM::setc()
{
    uint32_t value = get_from_stack(*PC);
    *CX = (uint8_t)value;
}

void VM::loop(uint8_t address)
{
    *CX--;
    if(*CX > 0){
        *PC = address;
    }
    else{
        *PC += 1;
    }
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
