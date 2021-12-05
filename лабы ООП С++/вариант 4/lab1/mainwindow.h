#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Recipe.h"
#include <QMainWindow>
#include "Queue.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Recipe;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    friend class Queue<std::shared_ptr<Item>>;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionSave_triggered();

    void on_actionOpen_triggered();

    void on_AddIngridient_clicked();

    void on_AddOperation_clicked();

    void Update();
    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

private:
    Recipe *recipe;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
