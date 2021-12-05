#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialognew.h"
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->butLoad,SIGNAL(clicked()),this,SLOT(load())) ;
    connect(ui->butSave,SIGNAL(clicked()),this,SLOT(save())) ;
    connect(ui->butAdd,SIGNAL(clicked()),this,SLOT(add())) ;
    connect(ui->butDel,SIGNAL(clicked()),this,SLOT(del())) ;
    connect(ui->butExist,SIGNAL(clicked()),this,SLOT(exist())) ;
    connect(ui->butClear,SIGNAL(clicked()),this,SLOT(clear())) ;
    hash2table() ;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::hash2table()
{
    QList<QPair<QString,int>> tmp ;
    for (auto k: hash.getAllKeys())
        tmp.append(QPair<QString,int>(k,hash[k])) ;
    std::sort(tmp.begin(),tmp.end(),[] (QPair<QString,int> const& a, QPair<QString,int> const& b) {
        return a.second > b.second; }) ;

    ui->tableWidget->setRowCount(hash.getCount()) ;
    ui->tableWidget->setColumnCount(2) ;    
    for (int i=0; i<tmp.size(); i++) {
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(tmp[i].first)) ;
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString::number(tmp[i].second))) ;
    }
    ui->tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("Элемент")) ;
    ui->tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("Цена")) ;

    ui->labInfo->setText(QString("Кол-во: %1<br>сумма: %2").arg(hash.getCount()).arg(hash.getSum())) ;

    ui->butDel->setEnabled(hash.getCount()>0) ;
    ui->butSave->setEnabled(hash.getCount()>0) ;
    ui->butExist->setEnabled(hash.getCount()>0) ;
    ui->butClear->setEnabled(hash.getCount()>0) ;
}

void MainWindow::load()
{
    QString filename = QFileDialog::getOpenFileName(this, "Загрузка") ;
    if (filename!="") {
        hash.loadFromFile(filename) ;
        hash2table() ;
    }
}

void MainWindow::save()
{
    QString filename = QFileDialog::getSaveFileName(this, "Запись") ;
    if (filename!="") hash.saveToFile(filename) ;
}

void MainWindow::add()
{
    DialogNew d(this) ;
    if (d.exec()==QDialog::Accepted) {
        hash.add(d.getName(),d.getPrice()) ;
        hash2table() ;
    }
}

void MainWindow::del()
{
    QString str = QInputDialog::getText(this,"Удаление","Введите имя для удаления") ;
    if (str!="") {
        hash.deleteKey(str) ;
        hash2table() ;
    }
}

void MainWindow::exist()
{
    QString str = QInputDialog::getText(this,"Проверка","Введите имя для проверки в коллекции") ;
    if (str!="")
        QMessageBox::information(this,"Результат проверки","Ключ "+str+" "+(hash.isContain(str)?"найден":"не найден")) ;
}

void MainWindow::clear()
{
    hash.deleteAll() ;
    hash2table() ;
}

