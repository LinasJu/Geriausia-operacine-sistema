#include "vm.h"

VM::VM(MemoryBlock* memory[], Cpu* cpu)
{

    std::copy(memory, memory+16, this->memory);
    this->cpu = cpu;
    this->CX = &(this->cpu->CX);
    this->PC = &(this->cpu->PC);
    this->SP = &(this->cpu->SP);
    this->PID = &(this->cpu->PID);
}

void VM::next()
{
    uint32_t cmd = get_from_memory(cpu->getPC1());

    switch (cmd) {
    case ('A' << 24) + ('D' << 16) + ('D' << 8) + ' ':
        add();
        cpu->incPC();
        break;
    case ('S' << 24) + ('U' << 16) + ('B' << 8) + ' ':
        sub();
        cpu->incPC();
        break;
    case ('M' << 24) + ('U' << 16) + ('L' << 8) + ' ':
        mul();
        cpu->incPC();
        break;
    case ('D' << 24) + ('I' << 16) + ('V' << 8) + ' ':
        div();
        cpu->incPC();
        break;
    case ('C' << 24) + ('M' << 16) + ('P' << 8) + ' ':
        cmp();
        cpu->incPC();
        break;
    case ('S' << 24) + ('E' << 16) + ('T' << 8) + 'C':
        setc();
        cpu->incPC();
        break;
    case ('P' << 24) + ('R' << 16) + ('T' << 8) + 'N':
        prtn();
        cpu->incPC();
        break;
    case ('P' << 24) + ('R' << 16) + ('T' << 8) + 'S':
        prts();
        cpu->incPC();
        break;
    case ('H' << 24) + ('A' << 16) + ('L' << 8) + 'T':
        halt();
        cpu->incPC();
        break;
    default:
        break;
    }
    switch ((cmd & 0xFFFF0000 ) >> 16) {
    case ('L' << 8) + 'D':
        ld((uint8_t)cmd&0x00FF);
        cpu->incPC();
        break;
    case ('P' << 8) + 'T':
        pt((uint8_t)cmd&0x00FF);
        cpu->incPC();
        break;
    case ('P' << 8) + 'S':
        ps((uint8_t)cmd&0x000000FF);
        cpu->incPC();
        break;
    case ('J' << 8) + 'P':
        jp((uint8_t)cmd&0x00FF);
        break;
    case ('J' << 8) + 'E':
        je((uint8_t)cmd&0x00FF);
        break;
    case ('J' << 8) + 'N':
        jn((uint8_t)cmd&0x00FF);
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
        cpu->setPI(2);
        break;
    }
}

void VM::run()
{
    while(*PC <= 111){
        next();
    }
}

void VM::save()
{
    if(temp != NULL){
        delete temp;
    }
    temp = new Temp();
    temp->CX = *CX;
    temp->PC = *PC;
    temp->PID = *PID;
    temp->SP = *SP;
}

void VM::load()
{
    if(temp != NULL){
        cpu->setCX(temp->CX);
        cpu->setPC(temp->PC);
        cpu->setPID(temp->PID);
        cpu->setSP(temp->SP);
        this->CX = &(this->cpu->CX);
        this->PC = &(this->cpu->PC);
        this->SP = &(this->cpu->SP);
        this->PID = &(this->cpu->PID);
    }
}

uint32_t VM::get_from_stack(uint8_t address)
{
    uint8_t first = ((address & 0xF0) + 0xE0) >> 4;
    if(first < 0x0E){
        cpu->setPI(1);
    }
    uint8_t second = address & 0x0F;
    return memory[first]->get(second);
}

void VM::set_to_stack(uint8_t address, uint32_t value)
{
    uint8_t first = ((address & 0xF0) + 0xE0) >> 4;
    uint8_t second = address & 0x0F;
    if(first < 0x0E){
        cpu->setPI(1);
    }
    memory[first]->set(second, value);
}

uint32_t VM::get_from_memory(uint8_t address)
{
    uint8_t first = (address & 0xF0) >> 4;
    uint8_t second = address & 0x0F;
    return memory[first]->get(second);
}

void VM::set_to_memory(uint8_t address, uint32_t value)
{
    uint8_t first = (address & 0xF0) >> 4;
    uint8_t second = address & 0x0F;
    memory[first]->set(second, value);
}

uint32_t VM::get_from_data(uint8_t address)
{
    uint8_t first = ((address & 0xF0) + 0x70) >> 4;
    if(first > 0x0E && first < 0x07){
        cpu->setPI(1);
    }
    uint8_t second = address & 0x0F;
    return memory[first]->get(second);
}

void VM::set_to_data(uint8_t address, uint32_t value)
{
    uint8_t first = ((address & 0xF0) + 0x70) >> 4;
    if(first > 0x0E && first < 0x07){
        cpu->setPI(1);
    }
    uint8_t second = address & 0x0F;
    memory[first]->set(second, value);
}

void VM::add()
{
    uint32_t a = get_from_stack(cpu->getSP1minus1());
    uint32_t b = get_from_stack(cpu->getSP1());

    uint64_t c = a & 0x80000000 ? (uint64_t)(0xFFFFFFFF00000000 | (uint64_t)a) : (uint64_t)a;
    uint64_t d = b & 0x80000000 ? (uint64_t)(0xFFFFFFFF00000000 | (uint64_t)b) : (uint64_t)b;

    cpu->clearSF();

    uint64_t result = c + d;


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

    cpu->decSP();
}

void VM::sub()
{
    uint32_t a = get_from_stack(cpu->getSP1minus1());
    uint32_t b = get_from_stack(cpu->getSP1());

    uint64_t c = a & 0x80000000 ? (uint64_t)(0xFFFFFFFF00000000 | (uint64_t)a) : (uint64_t)a;
    uint64_t d = b & 0x80000000 ? (uint64_t)(0xFFFFFFFF00000000 | (uint64_t)b) : (uint64_t)b;

    cpu->clearSF();

    uint64_t result = c - d;

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

    set_to_stack(cpu->getSP1minus1(), (uint32_t)(result&0xFFFFFFFF));

    cpu->decSP();
}

void VM::mul()
{
    uint32_t a = get_from_stack(cpu->getSP1minus1());
    uint32_t b = get_from_stack(cpu->getSP1());

    uint64_t c = a & 0x80000000 ? (uint64_t)(0xFFFFFFFF00000000 | (uint64_t)a) : (uint64_t)a;
    uint64_t d = b & 0x80000000 ? (uint64_t)(0xFFFFFFFF00000000 | (uint64_t)b) : (uint64_t)b;

    cpu->clearSF();

    uint64_t result = c * d;

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


    set_to_stack(*SP - 1, (uint32_t)(result & 0xFFFFFFFF));

    cpu->decSP();
}

void VM::div()
{
    uint32_t a = get_from_stack(cpu->getSP1minus1());
    uint32_t b = get_from_stack(cpu->getSP1());

    uint64_t c = a & 0x80000000 ? (uint64_t)(0xFFFFFFFF00000000 | (uint64_t)a) : (uint64_t)a;
    uint64_t d = b & 0x80000000 ? (uint64_t)(0xFFFFFFFF00000000 | (uint64_t)b) : (uint64_t)b;

    cpu->clearSF();

    uint64_t result = c / d;

    if(!b){
        cpu->setPI(4);
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

    set_to_stack(cpu->getSP1minus1(), (uint32_t)(result&0xFFFFFFFF));

    cpu->decSP();
}

void VM::cmp()
{
    uint32_t a = get_from_stack(cpu->getSP1minus1());
    uint32_t b = get_from_stack(cpu->getSP1());

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
    uint32_t value = get_from_data(address);
    set_to_stack(*SP+1, value);
    cpu->incSP();
}

void VM::pt(uint8_t address)
{
    uint32_t value = get_from_stack(cpu->getSP1());
    set_to_memory(address, value);
    cpu->incSP();
}

void VM::ps(uint8_t value)
{
    set_to_stack(*SP+1, value);
    cpu->incSP();
}

void VM::jp(uint8_t address)
{
    cpu->setPC1(address);
}

void VM::je(uint8_t address)
{
    if(cpu->getZF()){
        cpu->setPC1(address);
    }
}

void VM::jn(uint8_t address)
{
    if(!cpu->getZF()){
        cpu->setPC1(address);
    }
}

void VM::jl(uint8_t address)
{
    if(cpu->getSiF() == !cpu->getOF()){
        cpu->setPC1(address);
    }
}

void VM::jg(uint8_t address)
{
    if(cpu->getSiF() == cpu->getOF()){
        cpu->setPC1(address);
    }
}

void VM::setc()
{
    uint32_t value = get_from_stack(cpu->getSP1());
    *CX = (uint8_t)value;
}

void VM::loop(uint8_t address)
{
    *CX--;
    if(*CX > 0){
        cpu->setPC1(address);
    }
    else{
        cpu->incPC();
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
    cpu->setSI(4);
}

void VM::stop()
{
    cpu->setSI(4);
}

void VM::prts()
{
    cpu->setSI(1);
}

void VM::prtn()
{
    cpu->setSI(2);
}
