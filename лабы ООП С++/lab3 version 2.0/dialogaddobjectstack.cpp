#include "dialogaddobjectstack.h"
#include "ui_dialogaddobjectstack.h"
#include <QPushButton>

DialogAddObjectStack::DialogAddObjectStack(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddObjectStack)
{

    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

DialogAddObjectStack::~DialogAddObjectStack(){
    delete ui;
}

ObjectStack DialogAddObjectStack::getObject() const{
    return ObjectStack(ui->lineName->text().trimmed().toStdString(),
                ui->linePrice->text().toInt()) ;
}

void DialogAddObjectStack::onClickAccept(){
    accept();
}

void DialogAddObjectStack::onClickCancel(){
    reject();
}

void DialogAddObjectStack::on_lineName_textChanged(const QString &arg1)
{
    if(ui->lineName->text().isEmpty() || ui->linePrice->text().isEmpty()){
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    } else {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }

}

void DialogAddObjectStack::on_linePrice_textChanged(const QString &arg1)
{
    if(ui->lineName->text().isEmpty() || ui->linePrice->text().isEmpty()){
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    } else {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}
