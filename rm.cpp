#include "rm.h"

RM::RM()
{
    printer = new Printer();
    cp = new Cpu();
    mem=new Memory();
    flash= new FlashReader();
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
void RM::inputInterrupt(){

}
void RM::outputInterrupt(){

}
void RM::timerInterrupt(){
    this->cp->setTI(10);
}
void RM::programInterrupt(){
    delete vms[this->cp->getPC2()-1];
    if(this->cp->getPI()==1){
        w->appendOutput("Invalid address");}
    else if(this->cp->getPI()==2){
        w->appendOutput("Invalid operation code");
    }
    else if(this->cp->getPI()==3){
        w->appendOutput("Invalid assignment");
    }
    else{
        w->appendOutput("Overflow");
    }
    this->cp->setPI(0);

}
void RM::supervisorInterrupt(){
    if(this->cp->getSI()==1){
        this->printer->prints(this->mem->get(getStackPosition()),this->w);
        this->cp->setSI(0);
    }
    else if(this->cp->getSI()==2){
        this->printer->print(this->mem->get(getStackPosition()),this->w);
        this->cp->setSI(0);
    }
    else if(this->cp->getSI()==3){
    //READ
    }
    else{
        delete vms[this->cp->getPC2()-1];
    }

}

void RM::initWindow(MainWindow &w){
    this->w=&w;
}
int RM::getStackPosition(){
    return (mem->getMemoryBlock((this->cp->getSP2())).get(((this->cp->getSP1() & 0xF0) >>4) ))*16+(this->cp->getSP1() & 0x0F);
}
void RM::insertFlash(std::string path){
    flash->connectToDevice(path);
}
