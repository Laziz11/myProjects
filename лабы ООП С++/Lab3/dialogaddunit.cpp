#include "dialogaddunit.h"
#include "ui_dialogaddunit.h"

DialogAddUnit::DialogAddUnit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddUnit)
{
    ui->setupUi(this);
}

DialogAddUnit::~DialogAddUnit()
{
    delete ui;
}

void DialogAddUnit::on_buttonBox_accepted()
{
    accept() ;
}

void DialogAddUnit::on_buttonBox_rejected()
{
    reject() ;
}

Unit DialogAddUnit::getUnit() const
{
    return Unit(ui->lineName->text().trimmed().toStdString(),
                ui->linePrice->text().toInt(),
                ui->lineVolume->text().toInt()) ;
}
