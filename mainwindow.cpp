#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(uint8_t *pid,uint16_t *pc,uint16_t *sp, uint8_t *cx,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->pid=pid;
    this->pc=pc;
    this->sp=sp;
    this->cx=cx;
    ui->setupUi(this);
    QMainWindow::showMaximized();
    ui->tableVM->horizontalHeader()->setSectionResizeMode (QHeaderView::Fixed);
    ui->tableVM->verticalHeader()->setSectionResizeMode (QHeaderView::Fixed);
    initTable();
    initRealMemoryTable();
    addToTable(0,0,0x4b61726f);
    addToTable(0,1,0x6c697320);
    addToTable(0,2,0x79726120);
    addToTable(0,3,0x67616964);
    addToTable(0,4,0x79730000);

}
void MainWindow::initRealMemoryTable(){
    std::stringstream stream;
    std::string result;
    while(ui->tableRM->rowCount()<=0xFFF){
    int number = ui->tableRM->rowCount();
    stream << std::hex << std::uppercase << std::setfill('0') << std::setw(3) << number;
    std::string result( stream.str() );
    stream.str(std::string());
    ui->tableRM->insertRow( ui->tableRM->rowCount() );
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    ui->tableRM->setItem(number, 0, item);
    item = new QTableWidgetItem();
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    ui->tableRM->setItem(number, 1, item);
    item = new QTableWidgetItem();
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    ui->tableRM->setItem(number, 2,item);
    ui->tableRM->setVerticalHeaderItem(number, new QTableWidgetItem(QString::fromStdString(result)));
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
    std::stringstream stream;
    stream << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << item;
    std::string result( stream.str() );
    ui->tableVM->item(row, column)->setText(QString::fromStdString(result));
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
    ui->lineEdit_22->setText(QString::number( *pc ) );
    ui->lineEdit_21->setText(QString::number( *pid ));
    ui->lineEdit_23->setText(QString::number( *sp ));
    ui->lineEdit_24->setText(QString::number( *cx ));
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
    row=*pc/16;
    column=*pc%16;
    ui->tableVM->item(row, column)->setBackground(Qt::red);
    row=(*sp%256)/16 + 13;
    column=(*sp%256)%16;
    ui->tableVM->item(row, column)->setBackground(Qt::red);
}

void MainWindow::on_pushButton_3_clicked()
{
    this->initTable();
}
