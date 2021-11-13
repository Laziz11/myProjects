#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QtGui>
#include <QtWidgets>
#include <QWidget>
#include <QLabel>
#include <QTableWidget>

#include "Winners.h"
#include "WinnersModel.h"

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();

private:
    QTableView *tableStudents;
    QLabel *labelFIO;
    QLineEdit *editFIO;
    QLabel *labelWins;
    QLineEdit *editWins;
    QPushButton *buttonSave;
    QPushButton *buttonInsert;
    QPushButton *buttonDelete;
    QListWidget *listWinners;

    WinnersModel *myModel;  // Модель данных
    winners w;
    QString key;	// Ключ выделенной строки

    void resetWinners();	// Пересчитать победителей
    void update();	// Обновить данные в таблице студентов

private slots:
    void selectionChangedSlot(const QItemSelection &newSelection, const QItemSelection &oldSelection);
    void insertSlot();
    void deleteSlot();
    void saveSlot();
};

#endif // MAINDIALOG_H
