#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogaddobject.h"
#include "dialogaddobjectstack.h"
#include "dialogisneedtosaveold.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    updateList();
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::updateList(){
    connect(ui->listObjects, SIGNAL(clicked(QModelIndex)), this,
            SLOT(on_listWidget_clicked(QModelIndex)));
    ui->listObjects->clear();
    ui->butDelItem->setDisabled(true);
    ui->butDeleteAll->setDisabled(true);
    for (Object & u: list){
        ui->listObjects->addItem(QString::fromStdString(u.getObjectTitle()));
    }
    if(ui->listObjects->count()>0){
        ui->butSave->setDisabled(false);
        ui->butDeleteAll->setDisabled(false);
    } else {
        ui->butSave->setDisabled(true);
        ui->butDeleteAll->setDisabled(true);
    }

}

void MainWindow::on_butAddObject_clicked(){
    DialogAddObject d(this);
    if (d.exec()==QDialog::Accepted) {
        list.addObjectToItems(d.getObject());
        updateList();
    }
}

void MainWindow::on_butAddObjectStack_clicked(){
    DialogAddObjectStack d(this);
    if (d.exec()==QDialog::Accepted) {
        list.addObjectToItems(d.getObject());
        updateList();
    }
}

void MainWindow::on_butDelItem_clicked(){
    if (ui->listObjects->currentRow()==-1) {
       QMessageBox::information(this,"Error","No choosed element for deleting");
        return ;
    }
    list.deleteItemByIndex(ui->listObjects->currentRow());
    updateList();
}

void MainWindow::on_butDeleteAll_clicked(){
    list.clearList();
    updateList();
}

void MainWindow::on_listWidget_clicked(const QModelIndex &index){
  ui->butDelItem->setEnabled(true);
}

void MainWindow::on_butSave_clicked(){
    QString filename = QFileDialog::getSaveFileName(this, "Save",
                                                   QDir::currentPath(),"*.txt");
    if (filename=="") return;

    list.saveListToFile(filename.toStdString());
}

void MainWindow::on_butLoad_clicked(){
    if(ui->listObjects->count()>0){
        DialogIsneedToSaveOld d(this);
        if (d.exec()==QDialog::Accepted) {
            QString filename = QFileDialog::getSaveFileName(this, "Save",
                                                   QDir::currentPath(),"*.txt");
            if (filename!=""){
                list.saveListToFile(filename.toStdString());
                updateList();
                filename = QFileDialog::getOpenFileName(this, "Open",
                                                   QDir::currentPath(),"*.txt");
                if (filename=="") return;
                list.loadListFromFile(filename.toStdString());
                updateList();
            }

        }
    } else {
        QString filename = QFileDialog::getOpenFileName(this, "Open",
                                                   QDir::currentPath(),"*.txt");
        if (filename=="") return;
        list.loadListFromFile(filename.toStdString());
        updateList();
    }
}
