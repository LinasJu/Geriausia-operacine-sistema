#include "rm.h"

RM::RM()
{
    cp = new Cpu();
    mem=new Memory();
    this->initPageTable();
    this->newTable();//testing
}
void RM::addVM(){
    MemoryBlock* memory[16];
    for(uint8_t i=0;i<16;i++){
        *memory[i]=this->mem->getMemoryBlock(mem->get((((this->cp->getPC()&0xFF00)>>8)-1)*16+i));//change tempPC to CPU PC
    }
}
bool RM::newTable()
{
    if(cp->getPC()==16){
        return false;
    }
    srand(time(0));
    uint16_t r;
    uint16_t address;
    for(uint8_t i=0; i<16;i++){
        r= (rand() % 241) + 17;
        while(isTaken(r,i)){
            r= (rand() % 241) + 17;
        }
        address=(((this->cp->getPC()&0xFF00)>>8)-1);
        this->mem->set(address*16+i,r);//change tempPC to CPU PC

    }
    return true;
}
bool RM::isTaken(uint32_t block, uint8_t curr)
{
    for(int i = (((this->cp->getPC()&0xFF00)>>8)-1)*16+curr ; i >=0;i--){
        //change tempPC to CPU PC
        if(this->mem->get(i) ==block) return true;
    }
    return false;
}
void RM::initPageTable(){
    for(int i = 0 ; i<16;i++){
        MemoryBlock *memory=new MemoryBlock();
        for(int j = 0;j<16;j++){
            memory->set(j,0);
        }
        this->mem->setMemoryBlock(memory,i);
    }
}
void RM::inputInterrupt(){}
void RM::outputInterrupt(){
//printer
}
void RM::timerInterrupt(){
    this->cp->setTI(10);
}
void RM::programInterrupt(){
    //vm.stop();
}
void RM::supervisorInterrupt(){

}
