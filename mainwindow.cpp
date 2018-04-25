#include "mainwindow.h"
#include "ui_mainwindow.h"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    realmachine = new RM();
    realmachine->initWindow(*this);
    ui->setupUi(this);
    QMainWindow::showMaximized();
    ui->tableVM->horizontalHeader()->setSectionResizeMode (QHeaderView::Fixed);
    ui->tableVM->verticalHeader()->setSectionResizeMode (QHeaderView::Fixed);
    initTable();
    initRealMemoryTable();
    this->ui->disconnectButton->setEnabled(false);
    this->ui->stepButton->setEnabled(false);
    this->ui->runButton->setEnabled(false);
    this->ui->lightbulb->setPixmap(QPixmap(qApp->applicationDirPath()+"/lightbulbOFF.png"));

//    addToTable(0,0,0x4b61726f);
//    addToTable(0,1,0x6c697320);
//    addToTable(0,2,0x79726120);
//    addToTable(0,3,0x67616964);
//    addToTable(0,4,0x79730000);

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
void MainWindow::initTable(){
    for(uint8_t i = 0; i<16 ; i++){
        for(uint8_t j = 0 ; j<16; j++){
             ui->tableVM->setItem(i, j, new QTableWidgetItem(QString("")));
             if(i<6){
                 ui->tableVM->item(i, j)->setBackground(Qt::blue);
             }
             else if(i<13){
                 ui->tableVM->item(i, j)->setBackground(Qt::green);
             }
             else{
                 ui->tableVM->item(i, j)->setBackground(Qt::yellow);
             }

        }
    }
    for(uint8_t i = 0; i<16 ; i++){
        for(uint8_t j = 0 ; j<16; j++){
            addToTable(i,j,0x00000000);
        }
    }
}
void MainWindow::addToTable(uint8_t row, uint8_t column, uint32_t item){
    ui->tableVM->item(row, column)->setText(QString::fromStdString(this->intToHexStr(item,8)));
    this->addToolTip(row,column,item);
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
    if(this->realmachine->lightbulb->getState())
        this->ui->lightbulb->setPixmap(QPixmap(qApp->applicationDirPath()+"/lightbulbON.png"));
    else
        this->ui->lightbulb->setPixmap(QPixmap(qApp->applicationDirPath()+"/lightbulbOFF.png"));
    this->updateRealTable();
    ui->PCV->setText(QString::fromStdString(this->intToHexStr(this->realmachine->cp->getPC(),4)));
    ui->PIDV->setText(QString::fromStdString(this->intToHexStr(this->realmachine->cp->getPID(),2)));
    ui->SPV->setText(QString::fromStdString(this->intToHexStr(this->realmachine->cp->getSP(),4)));
    ui->CXV->setText(QString::fromStdString(this->intToHexStr(this->realmachine->cp->getCX(),2)));
    ui->CXR->setText(QString::fromStdString(this->intToHexStr(this->realmachine->cp->getCX(),2)));
    ui->CH1R->setText(QString::fromStdString(this->intToHexStr(this->realmachine->cp->getCH1(),2)));
    ui->CH2R->setText(QString::fromStdString(this->intToHexStr(this->realmachine->cp->getCH2(),2)));
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
    for(uint8_t i = 0; i<16 ; i++){
        for(uint8_t j = 0 ; j<16; j++){
             if(i<6){
                 ui->tableVM->item(i, j)->setBackground(Qt::blue);
             }
             else if(i<13){
                 ui->tableVM->item(i, j)->setBackground(Qt::green);
             }
             else{
                 ui->tableVM->item(i, j)->setBackground(Qt::yellow);
             }

        }
    }
    for(uint8_t i = 0; i<16 ; i++){
        int k = this->realmachine->mem->get((this->realmachine->cp->getPC2()-1)*16 +i);
        for(uint8_t j = 0 ; j<16; j++){
             if(i<6){
                 ui->tableRM->item(k, j)->setBackground(Qt::blue);
             }
             else if(i<13){
                 ui->tableRM->item(k, j)->setBackground(Qt::green);
             }
             else{
                 ui->tableRM->item(k, j)->setBackground(Qt::yellow);
             }

        }
    }
    row=this->realmachine->cp->getPC1()/16;
    column=this->realmachine->cp->getPC1()%16;
    ui->tableVM->item(row, column)->setBackground(Qt::red);
    row=(this->realmachine->cp->getSP1()%256)/16 + 13;
    column=this->realmachine->cp->getSP1()%16;
    ui->tableVM->item(row, column)->setBackground(Qt::red);
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
    if(this->realmachine->cp->getPC2()){
    for(int i = 0; i<16;i++){
        ui->tableRM->item(((this->realmachine->cp->getPC()-1) &0xFF00)>>8,i)->setBackground(Qt::blue);
    }}
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
    if(this->realmachine->lightbulb->getState())
    this->realmachine->lightbulb->print(0);
    else
        this->realmachine->lightbulb->print(1);
    this->update();
}
