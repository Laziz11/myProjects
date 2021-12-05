#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "MyHash.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    MyHash<QString,int> hash;
    void hash2table() ;
private slots:
    void load() ;
    void save() ;
    void add() ;
    void del() ;
    void exist() ;
    void clear() ;
};
#endif // MAINWINDOW_H
