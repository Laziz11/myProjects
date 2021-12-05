#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Recipe.h"


#include <iostream>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    recipe = new Recipe();
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete recipe;
    delete ui;
}


void MainWindow::on_actionSave_triggered()
{
    QString file_name = QFileDialog::getSaveFileName(this, "Save the file");
    recipe->print(file_name.toStdString());
}

void MainWindow::on_actionOpen_triggered()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Open the file");
    recipe->read(file_name.toStdString());
    Update();
}

void MainWindow::on_AddIngridient_clicked()
{
    if (ui->Title->text().toStdString() == "") {
        QMessageBox::critical(0, "Error", "Add a title");
        return;
    }
    recipe->emplace(std::make_shared<Ingridient>(
                        ui->Title->text().toStdString(),
                        ui->Unit->currentText().toStdString(),
                        ui->Quantity->value()
                        )
                    );
    ui->Title->clear();
    ui->Quantity->setValue(0);
    ui->Unit->setCurrentText("");
    Update();
}

void MainWindow::on_AddOperation_clicked()
{
    recipe->emplace(std::make_shared<Operation>
                    (
                        ui->Action->currentText().toStdString(),
                        ui->Time->value()
                        )
                    );
    ui->Action->setCurrentText("");
    ui->Time->setValue(0);
    Update();
}

void MainWindow::Update() {
    ui->textBrowser->clear();

    auto tmp = recipe->front_;

    for (size_t i = 0; i < recipe->size_; ++i) {
        if (tmp->value()->type() == Type::INGRIDIENT) {
            auto obj = tmp->value();
            QString item = (QString("* ----- INGREDIENT ----- *\n")
                            +QString(" ")
                            +QString(obj->title().c_str())
                            +QString(" ")
                            +QString(obj->unit().c_str())
                            +QString(" ")
                            +QString(std::to_string(obj->quantity()).c_str())
                            +QString("\n"));
            ui->textBrowser->append(item);
        } else {
            auto obj = tmp->value();
            QString item = (QString("! ----- OPERATION: ----- !\n")
                            +QString(" ")
                            +QString(obj->action().c_str())
                            +QString(" ")
                            +QString((obj->time()))
                            +QString("\n"));
            ui->textBrowser->append(item);
        }
        tmp = tmp->next();
    }
}


void MainWindow::on_pushButton_3_clicked()
{
    recipe->clear();
    Update();
}

void MainWindow::on_pushButton_4_clicked()
{
    recipe->pop();
    Update();
}

void MainWindow::on_pushButton_clicked()
{
    if (recipe->isValid()) {
        QMessageBox::warning(0, "Validate", "Recipe is valid.");
    } else {
        QMessageBox::warning(0, "Validate", "Recipe isn't valid.");
    }
}
