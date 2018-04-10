#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode (QHeaderView::Fixed);
    ui->tableWidget->verticalHeader()->setSectionResizeMode (QHeaderView::Fixed);
}


MainWindow::~MainWindow()
{
    delete ui;
}
