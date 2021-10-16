#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "list.h"
#include "unit.h"
#include "unitstack.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_butAddUnit_clicked();

    void on_butAddUnitStack_clicked();

    void on_butDelItem_clicked();

    void on_butSave_clicked();

    void on_butLoad_clicked();

private:
    Ui::MainWindow *ui;
    List list ;
    void updateList() ;
};
#endif // MAINWINDOW_H
