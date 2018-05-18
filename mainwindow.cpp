#include "mainwindow.h"
#include "ui_mainwindow.h"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    for(int i = 0;i<14;i++){
        vgs[i]=NULL;
    }
    realmachine = new RM();
    realmachine->initWindow(*this);
    ui->setupUi(this);
    QMainWindow::showMaximized();
    scene= new QGraphicsScene(this);
    ui->gView->setScene(scene);
    rect = new QGraphicsRectItem();
    rect->setRect(0,0,100,100);
    rect->setPen( QPen( Qt::black, 1 ) );
    rect->setBrush( Qt::white );
    scene->addItem(rect);
    initRealMemoryTable();
    this->ui->disconnectButton->setEnabled(false);
    this->ui->stepButton->setEnabled(false);
    this->ui->pushButton_2->setEnabled(false);
    this->ui->runButton->setEnabled(false);
    this->ui->next->setEnabled(false);
    this->ui->tabWidget->removeTab(0);
    this->addTable();
    this->update();
}
void MainWindow::initRealMemoryTable(){
    QTableWidgetItem *item = new QTableWidgetItem();
    while(ui->tableRM->rowCount()<=0xFF){
    int number = ui->tableRM->rowCount();
    ui->tableRM->insertRow( ui->tableRM->rowCount() );
    for(int i=0;i<16;i++){
        item = new QTableWidgetItem();
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        ui->tableRM->setItem(number, i, item);

    }
    ui->tableRM->setVerticalHeaderItem(number, new QTableWidgetItem(QString::fromStdString(this->intToHexStr(number,2))));
    }

}

void MainWindow::addTable(std::string str)
{
    virtualGUI *vg= new virtualGUI();
    vgs[this->realmachine->cp->getPID()]=vg;
    this->ui->tabWidget->addTab(vg,QString::fromStdString(str));
}

void MainWindow::removeTable()
{
    this->ui->tabWidget->removeTab(this->realmachine->cp->getPID()+1);
    delete vgs[this->realmachine->cp->getPID()];
    for(int i = this->realmachine->cp->getPID();i<14;i++){
        vgs[i]=vgs[i+1];
    }
    vgs[14]=NULL;
}
void MainWindow::addToTable(uint8_t row, uint8_t column, uint32_t item){
    vgs[this->realmachine->cp->getPID()]->addToTable(row,column,item);
}
void MainWindow::addToolTip(uint8_t row, uint8_t column, uint32_t item){
    char *ptr = (char*)&item;
    char *msg = new char[4];
    for(int i=0;i<4;++i, ++ptr)
        msg[3-i] = *ptr;
    int c;
    std::string buffer="";
    c=0;
    while(c<4) {
        if(msg[c]>=32)
      buffer.append(1,msg[c]);
      else buffer.append(1,'ą');
      ++c;
    }
    ui->tableVM->item(row, column)-> setData(3,QString::fromStdString(buffer));
    this->headerToolTip(row);

}
void MainWindow::headerToolTip(uint8_t row){
    QString qs = ui->tableVM->item(row,0)->toolTip();
    for(int i = 1 ; i<16;i++){
        qs+=ui->tableVM->item(row,i)->toolTip();
    }
    ui->tableVM->verticalHeaderItem(row)->setData(3,qs);
}


MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::update(){
    this->changeColor(this->realmachine->lightbulb->getState());
    this->updateRealTable();
    int pid = this->realmachine->cp->getPID();
    if(vgs[pid]){
    vgs[pid]->setPC(QString::fromStdString(this->intToHexStr(this->realmachine->cp->getPC(),4)));
    vgs[pid]->setPID(QString::fromStdString(this->intToHexStr(this->realmachine->cp->getPID(),2)));
    vgs[pid]->setSP(QString::fromStdString(this->intToHexStr(this->realmachine->cp->getSP(),4)));
    vgs[pid]->setCX(QString::fromStdString(this->intToHexStr(this->realmachine->cp->getCX(),2)));
    }
    ui->CXR->setText(QString::fromStdString(this->intToHexStr(this->realmachine->cp->getCX(),2)));
    ui->CH1R->setText(QString::fromStdString(this->intToHexStr(this->realmachine->cp->getCH1(),2)));
    ui->CH2R->setText(QString::fromStdString(this->intToHexStr(this->realmachine->cp->getCH2(),2)));
    ui->CH2R->setText(QString::fromStdString(this->intToHexStr(this->realmachine->cp->getCH4(),2)));
    ui->CH3R->setText(QString::fromStdString(this->intToHexStr(this->realmachine->cp->getCH3(),2)));
    ui->PLRR->setText(QString::fromStdString(this->intToHexStr(this->realmachine->cp->getPLR(),8)));
    ui->PCR->setText(QString::fromStdString(this->intToHexStr(this->realmachine->cp->getPC(),4)));
    ui->SPR->setText(QString::fromStdString(this->intToHexStr(this->realmachine->cp->getSP(),4)));
    ui->IOIR->setText(QString::fromStdString(this->intToHexStr(this->realmachine->cp->getIOI(),2)));
    ui->SFR->setText(QString::fromStdString(this->intToHexStr(this->realmachine->cp->getSF(),2)));
    ui->PIDR->setText(QString::fromStdString(this->intToHexStr(this->realmachine->cp->getPID(),2)));
    ui->PIR->setText(QString::fromStdString(this->intToHexStr(this->realmachine->cp->getPI(),2)));
    ui->MODER->setText(QString::fromStdString(this->intToHexStr(this->realmachine->cp->getMODE(),2)));
    ui->TIR->setText(QString::fromStdString(this->intToHexStr(this->realmachine->cp->getTI(),2)));
    ui->SIR->setText(QString::fromStdString(this->intToHexStr(this->realmachine->cp->getSI(),2)));
    int row;
    int column;
    if(vgs[pid]){
    for(uint8_t i = 0; i<16 ; i++){
        for(uint8_t j = 0 ; j<16; j++){
             if(i<7){
                 vgs[pid]->setCellColour(i,j,Qt::blue);
             }
             else if(i<14){
                 vgs[pid]->setCellColour(i,j,Qt::green);
             }
             else{
                 vgs[pid]->setCellColour(i,j,Qt::yellow);
             }

        }
    }}
    if((this->realmachine->cp->getPC2()-1)>0){
    for(uint8_t i = 0; i<16 ; i++){
        int k = this->realmachine->mem->get((this->realmachine->cp->getPC2()-1)*16 +i);
        if(k>0xff){
        for(uint8_t j = 0 ; j<16; j++){
             if(i<7){
                 ui->tableRM->item(k, j)->setBackground(Qt::blue);
             }
             else if(i<14){
                 ui->tableRM->item(k, j)->setBackground(Qt::green);
             }
             else{
                 ui->tableRM->item(k, j)->setBackground(Qt::yellow);
             }

        }}
    }}
    row=this->realmachine->cp->getPC1()/16;
    column=this->realmachine->cp->getPC1()%16;
    if(row<16 && column <16 && vgs[pid]){
    vgs[pid]->setCellColour(row,column,Qt::red);
    }
    row=(this->realmachine->cp->getSP1()%256)/16 + 14;
    column=this->realmachine->cp->getSP1()%16;
    if(row<16 && column <16 && vgs[pid]){
    vgs[pid]->setCellColour(row,column,Qt::red);
    }
    int table=(realmachine->cp->getPC()&0xFF00)>>8;
    if(table>0){
    for(int i=0;i<16;i++){
        int block=this->realmachine->mem->get((table-1)*16+i);
        for(int j=0;j<16;j++){
            this->addToTable(i,j,this->realmachine->mem->get(block*16+j));
        }
    }
    }
}
void MainWindow::updateRealTable()
{
    int row;
    int column;
    for(uint16_t i =0;i<0x1000;i++){
        column=i&0xF;
        row=(i&0xFF0) >> 4;
        ui->tableRM->item(row,column)->setText(QString::fromStdString(this->intToHexStr(this->realmachine->mem->get(i),8)));
    }
    for(int j = 0;j<16;j++){
    for(int i = 0; i<16;i++){
        ui->tableRM->item(j,i)->setBackground(Qt::white);
    }}
    if(this->realmachine->cp->getPC2()){
    for(int i = 0; i<16;i++){
        ui->tableRM->item(((this->realmachine->cp->getPC2()-1)),i)->setBackground(Qt::blue);
    }}
}

void MainWindow::resetRMcolor()
{
    for(int i = 0; i<16; i++){
        for(int j=0;j<256;j++){
         ui->tableRM->item(j,i)->setBackground(Qt::white);
        }
    }
}

void MainWindow::appendOutput(std::string str){
    QString qs;
    qs=this->ui->textBrowser->toPlainText();
    qs+=QString::fromStdString(str);
    this->ui->textBrowser->setText(qs);
}

void MainWindow::on_connectButton_clicked()
{
    QString filename=QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "C://",
                "All files (*.*)"
                );
    this->realmachine->insertFlash(filename.toStdString());
    this->ui->checkBox->setChecked(true);
    this->ui->connectButton->setEnabled(false);
    this->ui->disconnectButton->setEnabled(true);
    this->ui->pushButton_2->setEnabled(true);

}
std::string MainWindow::intToHexStr(int a,int pos){
    std::stringstream stream;
      stream << std::setfill ('0') << std::setw(pos) << std::hex << a;
      return stream.str();
}

void MainWindow::on_disconnectButton_clicked()
{
    this->ui->connectButton->setEnabled(true);
    this->ui->disconnectButton->setEnabled(false);
    this->ui->checkBox->setChecked(false);
}

void MainWindow::on_checkBox_clicked()
{
    this->ui->checkBox->setChecked(!this->ui->checkBox->isChecked());
}

void MainWindow::on_stepButton_clicked()
{
    if(this->realmachine->cp->getPC2()>0){
    this->realmachine->next();}
}

void MainWindow::on_runButton_clicked()
{
    realmachine->stop=false;
    this->realmachine->run();
}
void MainWindow::changeRunButtonState(bool a){
    this->ui->runButton->setEnabled(a);
}
void MainWindow::changeStepButtonState(bool a){
    this->ui->stepButton->setEnabled(a);
}

void MainWindow::on_pushButton_clicked()
{
   realmachine->flash->disconnectFromDevice();
   this->ui->connectButton->setEnabled(true);
   this->ui->disconnectButton->setEnabled(false);
   delete this->realmachine;
   realmachine = new RM();
   realmachine->initWindow(*this);
   update();
}
void MainWindow::changeColor(uint32_t color){
    int r=(color&0xFF000000)>>24;
    int g=(color&0x00FF0000)>>16;
    int b=(color&0x0000FF00)>>8;
    int a=(color&0x000000FF);
    this->rect->setBrush(QBrush(QColor(r,g,b,a)));
}

void MainWindow::on_pushButton_2_clicked()
{
    realmachine->loadFlashToSupervisorMemory();
    this->ui->stepButton->setEnabled(true);
    this->ui->runButton->setEnabled(true);
    realmachine->addVM();
    realmachine->cp->setSP(0);
    realmachine->cp->setPC1(0);
    this->update();
    this->ui->pushButton_2->setEnabled(false);
}

void MainWindow::on_next_clicked()
{
    realmachine->getNext();
    this->update();
    this->ui->next->setEnabled(false);
    this->ui->stepButton->setEnabled(true);
    this->ui->runButton->setEnabled(true);
}
void MainWindow::changeNextButton(bool a){
    this->ui->next->setEnabled(a);
}

void MainWindow::on_stop_clicked()
{
    realmachine->stop=true;
}
