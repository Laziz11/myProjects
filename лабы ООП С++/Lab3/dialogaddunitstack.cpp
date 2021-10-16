#include "dialogaddunitstack.h"
#include "ui_dialogaddunitstack.h"

DialogAddUnitStack::DialogAddUnitStack(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddUnitStack)
{
    ui->setupUi(this);
}

DialogAddUnitStack::~DialogAddUnitStack()
{
    delete ui;
}

UnitStack DialogAddUnitStack::getUnit() const
{
    return UnitStack(ui->lineName->text().trimmed().toStdString(),
                ui->linePrice->text().toInt()) ;
}

void DialogAddUnitStack::on_buttonBox_accepted()
{
    accept() ;
}

void DialogAddUnitStack::on_buttonBox_rejected()
{
    reject() ;
}
