#include "virtualmachine.h"

VirtualMachine::VirtualMachine(MemoryBlock* memory)
{
    this->memory = memory;
}

void VirtualMachine::add()
{
    memory->set(*SP - 1, memory->get(*SP - 1) + memory->get(*SP));
    *SP--;
}

void VirtualMachine::sub()
{
    memory->set(*SP - 1, memory->get(*SP - 1) - memory->get(*SP));
    *SP--;
}

void VirtualMachine::mul()
{
    memory->set(*SP - 1, memory->get(*SP - 1) * memory->get(*SP));
    *SP--;
}

void VirtualMachine::div()
{
    memory->set(*SP - 1, memory->get(*SP - 1) / memory->get(*SP));
    *SP--;
}

void VirtualMachine::cmp()
{

}

void VirtualMachine::ld(uint16_t address)
{

}

void VirtualMachine::pt(uint16_t address)
{

}

void VirtualMachine::ps(uint16_t value)
{

}

void VirtualMachine::jp(uint16_t address)
{

}

void VirtualMachine::je(uint16_t address)
{

}

void VirtualMachine::jn(uint16_t address)
{

}

void VirtualMachine::jl(uint16_t address)
{

}

void VirtualMachine::jg(uint16_t address)
{

}

void VirtualMachine::setc()
{

}

void VirtualMachine::loop(uint16_t address)
{

}

void VirtualMachine::fork()
{

}

void VirtualMachine::isp()
{

}

void VirtualMachine::isc()
{

}

void VirtualMachine::halt()
{

}

void VirtualMachine::stop()
{

}

void VirtualMachine::prts()
{

}

void VirtualMachine::prtn()
{

}
