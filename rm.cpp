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

RM::~RM() {
    delete[] supervisorMemory;
}

void RM::addVM(){
    MemoryBlock* memory[16];
    for(uint8_t i=0;i<16;i++){
        *memory[i]=this->mem->getMemoryBlock(mem->get((((this->cp->getPC()&0xFF00)>>8)-1)*16+i));//change tempPC to CPU PC
    }
}
bool RM::newTable()
{

    if(cp->getPC()==15){
        return false;
    }
    this->cp->setPC2(this->cp->getPC2()+1);
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
        for(int j = 0;j<16;j++){
            this->mem->set(i*16+j,0);
        }
    }
}
void RM::inputOtputInterrupt(){
    this->cp->setIOI(0);
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
        if(vms[this->cp->getPC2()-1]!=NULL)
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
VM RM::getNext(){//sets current VM and returns it
    if((this->cp->getPC2()-1)>1){
    this->current=this->vms[this->cp->getPC2()-1];}
    return current;
}
void RM::next(){
    current->next();
    if(this->test()){
        if(this->cp->getPI()){
            this->programInterrupt();
        }
        if(this->cp->getSI()){
            this->supervisorInterrupt();
        }
        if(this->cp->getTI()<0){
            this->timerInterrupt();
        }
        if(this->cp->getIOI()){
            this->inputOutputInterrupt();
        }
    }
}
bool RM::test(){
    return ((this->cp->getPI() + this->cp->getIOI() + this->cp->getSI()) || this->cp->getTI()<=0);
}

void RM::readFromFlash(char *destination, uint16_t bytes) {
    if (flash->isConnected()) {
        flash->read(destination, bytes);
    }
}

void RM::loadFlashToSupervisorMemory() {
    uint16_t size = flash->getFileSize();
    char *buffer = nullptr;

    // allocated memory must be divisible by 4
    if (size % 4 != 0) {
        buffer = new char[size + size % 4];
    }
    else {
        buffer = new char[size];
    }

    readFromFlash(buffer, size);

    // deal with wrong file size
    if (size % 4 != 0) {
        for (int i = 0; i < size % 4; i++) {
            // '?' stands for bad byte
            *(buffer + i) = '?';
        }
    }

    // buffer -> supervisor
    supervisorSize = static_cast<uint16_t>((size + size % 4) / 4);
    supervisorMemory = new uint32_t[supervisorSize];
    for (int i = 0; i < supervisorSize; i++) {
        supervisorMemory[i] = static_cast<uint32_t>(
                (buffer[i] << 24) + (buffer[i + 1] << 16) + (buffer[i + 2] << 8) + buffer[i + 3]
        );
    }

    delete[] buffer;
}