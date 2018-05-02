#include "rm.h"
#include "parser_process.h"

RM::RM()
{
    printer = new Printer();
    cp = new Cpu();
    lightbulb = new Lightbulb();
    mem=new Memory();
    flash= new FlashReader();
    this->initPageTable();
    this->newTable();//testing
    MemoryBlock* vmmemory[16];
    int table=(this->cp->getPC()&0xFF00)>>8;
    if(table>0){
    for(int i=0;i<16;i++){
        int block=this->mem->get((table-1)*16+i);
//        for(int j=0;j<16;j++){
//            this->addToTable(i,j,this->mem->get(block*16+j));
//        }
        vmmemory[i]=mem->getMemoryBlockAddress(block);
    }
    }
    vmmemory[14]->set(1,0xFFFFFFFF);//testing
    this->cp->incSP();

    vmmemory[0]->set(0,0x48414C54);
    current = new VM(vmmemory,cp);
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
void RM::inputOutputInterrupt(){
    this->cp->setIOI(0);
}
void RM::timerInterrupt(){
    this->cp->setTI(10);
}
void RM::programInterrupt(){
    if(vms[this->cp->getPC2()-1]!=NULL ){
    delete vms[this->cp->getPC2()-1];}
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
    w->changeRunButtonState(false);
    w->changeStepButtonState(false);
}
void RM::supervisorInterrupt(){
    if(this->cp->getSI()==1){
        this->printer->prints(this->mem->get(getStackPosition()),this->w);
        this->cp->setSP1(this->cp->getSP1minus1());
        this->cp->setSI(0);
        this->w->update();
    }
    else if(this->cp->getSI()==2){
        this->printer->print(this->mem->get(getStackPosition()),this->w);
        this->cp->setSP1(this->cp->getSP1minus1());
        this->cp->setSI(0);
        this->w->update();
    }
    else if(this->cp->getSI()==3){
        this->cp->setSI(0);
    //READ
    }
    else if(this->cp->getSI()==5){
        this->lightbulb->print(this->mem->get(getStackPosition()));
        this->cp->setSP1(this->cp->getSP1minus1());
        this->cp->setSI(0);
        this->w->update();
    }
    else{
        if(vms[this->cp->getPC2()-1]!=NULL)
        delete vms[this->cp->getPC2()-1];
        w->changeRunButtonState(false);
        w->changeStepButtonState(false);

    }

}

void RM::initWindow(MainWindow &w){
    this->w=&w;
}
int RM::getStackPosition(){
    int page = mem->getMemoryBlock(this->cp->getPC2()-1 ).get(14 + ((this->cp->getSP1()&0xF0)>>4));
    int cell =(this->cp->getSP1() )&0x0F;
    int address=(page*16+cell);
    return address;
}
void RM::insertFlash(std::string path){
    flash->connectToDevice(path);
}
VM RM::getNext(){//sets current VM and returns it
    if((this->cp->getPC2()-1)>1){
    this->current=this->vms[this->cp->getPC2()-1];}
    return *current;
}
void RM::next(){
    if(this->cp->getMODE()==0){
    current->next();
    if(this->test()){
        this->cp->setMODE(1);
//        current->save();
    }
    }
    else{

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
        this->cp->setMODE(0);
//        if(current!=NULL)
//        current->load();
    }
    this->w->update();
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
                (buffer[4*i] << 24) + (buffer[4*i + 1] << 16) + (buffer[4*i + 2] << 8) + buffer[4*i + 3]
        );
    }

    delete[] buffer;
}

ParsedProgram *RM::parseProgram() {
    ParserProcess parser(supervisorMemory, supervisorSize);
    parser.parse();

    if (parser.isError()) {
        // error
        std::cout << parser.getParseLog() << std::endl;
        return nullptr;
    }
    else {
        ParsedProgram *prog = parser.getParsedProgram();
        //uint32_t masyvas[prog->getCodeSize()];
        for(int i = 0 ; i<prog->getCodeSize();i++){
            uint32_t g = *(prog->getCode()+i);
            this->mem->set((this->mem->get(this->cp->getPC2()-1+(i>>4))) *16 +i,*(prog->getCode()+i));
//            masyvas=*(prog->getCode()+i);
            }
        for(int i = 0 ; i<prog->getDataSize();i++){
            uint32_t g = *(prog->getData()+i);
            this->mem->set((this->mem->get(this->cp->getPC2()-1+(i>>4)+7)) *16 +i,*(prog->getData()+i));
//            masyvas=*(prog->getCode()+i);
            }
        return parser.getParsedProgram();
    }
}

void RM::run(){
    while((vms[this->cp->getPC2()-1]!=NULL )){
        this->next();
    }
}
