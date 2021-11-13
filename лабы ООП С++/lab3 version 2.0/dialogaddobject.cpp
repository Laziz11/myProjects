#include "dialogaddobject.h"
#include "ui_dialogaddobject.h"
#include <QPushButton>

DialogAddObject::DialogAddObject(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddObject){
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

DialogAddObject::~DialogAddObject(){
    delete ui;
}

void DialogAddObject::onClickAccept(){
    accept();
}

void DialogAddObject::onClickCancel(){
    reject();
}

Object DialogAddObject::getObject() const{
    return Object(ui->lineName->text().trimmed().toStdString(),
                ui->linePrice->text().toInt(),
                ui->lineVolume->text().toInt()) ;
}



void DialogAddObject::on_lineName_textChanged(const QString &arg1){
    if(ui->lineName->text().isEmpty() || ui->linePrice->text().isEmpty()||
            ui->lineVolume->text().isEmpty()){
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    } else {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }

}


void DialogAddObject::on_linePrice_textChanged(const QString &arg1){
    if(ui->lineName->text().isEmpty() || ui->linePrice->text().isEmpty()||
            ui->lineVolume->text().isEmpty()){
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    } else {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}


void DialogAddObject::on_lineVolume_textChanged(const QString &arg1){
    if(ui->lineName->text().isEmpty() || ui->linePrice->text().isEmpty()||
            ui->lineVolume->text().isEmpty()){
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    } else {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}

