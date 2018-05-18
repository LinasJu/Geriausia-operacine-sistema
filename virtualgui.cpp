#include "virtualgui.h"
#include "ui_virtualgui.h"
#include "mainwindow.h"
virtualGUI::virtualGUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::virtualGUI)
{
    ui->setupUi(this);
    ui->tableVM->horizontalHeader()->setSectionResizeMode (QHeaderView::Fixed);
    ui->tableVM->verticalHeader()->setSectionResizeMode (QHeaderView::Fixed);
    this->initTable();
}

virtualGUI::~virtualGUI()
{
    delete ui;
}

void virtualGUI::setCellColour(int row,int column,const QBrush &brush){
    ui->tableVM->item(row, column)->setBackground(brush);
}
void virtualGUI::addToTable(int row,int column,uint32_t value){
    ui->tableVM->item(row, column)->setText(QString::fromStdString(this->intToHexStr(value,8)));
    this->addToolTip(row,column,value);
}

void virtualGUI::setPID(QString a)
{
    this->ui->PIDV->setText(a);
}

void virtualGUI::setPC(QString a)
{
    this->ui->PCV->setText(a);
}

void virtualGUI::setSP(QString a)
{
    this->ui->SPV->setText(a);
}

void virtualGUI::setCX(QString a)
{
    this->ui->CXV->setText(a);
}

void virtualGUI::initTable(){
    for(uint8_t i = 0; i<16 ; i++){
        for(uint8_t j = 0 ; j<16; j++){
             ui->tableVM->setItem(i, j, new QTableWidgetItem(QString("")));
             if(i<7){
                 ui->tableVM->item(i, j)->setBackground(Qt::blue);
             }
             else if(i<14){
                 ui->tableVM->item(i, j)->setBackground(Qt::green);
             }
             else{
                 ui->tableVM->item(i, j)->setBackground(Qt::yellow);
             }

        }
    }
    for(uint8_t i = 0; i<16 ; i++){
        for(uint8_t j = 0 ; j<16; j++){
            this->addToTable(i,j,0x00000000);
        }
    }
}

void virtualGUI::addToolTip(uint8_t row, uint8_t column, uint32_t item){
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
void virtualGUI::headerToolTip(uint8_t row){
    QString qs = ui->tableVM->item(row,0)->toolTip();
    for(int i = 1 ; i<16;i++){
        qs+=ui->tableVM->item(row,i)->toolTip();
    }
    ui->tableVM->verticalHeaderItem(row)->setData(3,qs);
}
std::string virtualGUI::intToHexStr(int a,int pos){
    std::stringstream stream;
      stream << std::setfill ('0') << std::setw(pos) << std::hex << a;
      return stream.str();
}
