#include "rm.h"

RM::RM()
{
    this->mem=new Memory();
}


VM getCurrent(){
    idx = cpu->pc & 0xFF00 >> 8;
    return VMS[idx];
}

VM getExt(){
    idx = cpu->pc & 0xFF00 >> 8;
    idx ++;
    while (VMS[idx] == null) {
        idx ++;
    }
    return VMS[idx];
}


void RM:: timerInt(){
    this->TI=100;
    vm.save();
    addToQue(vm);
    vm=getNextVM();
    vm.load();

}
