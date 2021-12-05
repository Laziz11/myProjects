#include "dialognew.h"
#include "ui_dialognew.h"

DialogNew::DialogNew(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNew)
{
    ui->setupUi(this);
}

DialogNew::~DialogNew()
{
    delete ui;
}

QString DialogNew::getName() const
{
    return ui->edName->text() ;
}

int DialogNew::getPrice() const
{
    return ui->edPrice->value() ;
}
