#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogaddunit.h"
#include "dialogaddunitstack.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    updateList() ;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateList()
{
    ui->listUnits->clear() ;

    for (Unit & u: list)
        ui->listUnits->addItem(QString::fromStdString(u.getTitle())) ;
}


void MainWindow::on_butAddUnit_clicked()
{
    DialogAddUnit d(this) ;
    if (d.exec()==QDialog::Accepted) {
        list.insertUnit(d.getUnit()) ;
        updateList() ;
    }
}

void MainWindow::on_butAddUnitStack_clicked()
{
    DialogAddUnitStack d(this) ;
    if (d.exec()==QDialog::Accepted) {
        list.insertUnit(d.getUnit()) ;
        updateList() ;
    }
}

void MainWindow::on_butDelItem_clicked()
{
    if (ui->listUnits->currentRow()==-1) {
        QMessageBox::information(this,"Ошибка","Не выбран элемент для удаления") ;
        return ;
    }

    list.deleteIndex(ui->listUnits->currentRow()) ;
    updateList() ;
}

void MainWindow::on_butSave_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, "Сохранение", QDir::currentPath(),"*.dat") ;
    if (filename=="") return ;

    list.saveToFile(filename.toStdString()) ;
}

void MainWindow::on_butLoad_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Открытие", QDir::currentPath(),"*.dat") ;
    if (filename=="") return ;

    list.loadFromFile(filename.toStdString()) ;
    updateList() ;
}
