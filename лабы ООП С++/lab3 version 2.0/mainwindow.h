#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "list.h"
#include "object.h"
#include "objectstack.h"

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
    void on_butAddObject_clicked();

    void on_butAddObjectStack_clicked();

    void on_butDelItem_clicked();

    void on_butSave_clicked();

    void on_butLoad_clicked();

    void on_butDeleteAll_clicked();

    void on_listWidget_clicked(const QModelIndex &i);

private:
    Ui::MainWindow *ui;
    List list;
    void updateList();
};
#endif // MAINWINDOW_H
