#include "dialogisneedtosaveold.h"
#include "ui_dialogisneedtosaveold.h"
#include <QPushButton>

DialogIsneedToSaveOld::DialogIsneedToSaveOld(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogIsneedToSaveOld)
{
    ui->setupUi(this);
}

DialogIsneedToSaveOld::~DialogIsneedToSaveOld()
{
    delete ui;
}

void DialogIsneedToSaveOld::onClickAccept(){
    accept();
}

void DialogIsneedToSaveOld::onClickCancel(){
    reject();
}
