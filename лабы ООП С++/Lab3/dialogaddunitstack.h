#ifndef DIALOGADDUNITSTACK_H
#define DIALOGADDUNITSTACK_H

#include <QDialog>
#include "unitstack.h"

namespace Ui {
class DialogAddUnitStack;
}

class DialogAddUnitStack : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddUnitStack(QWidget *parent = nullptr);
    ~DialogAddUnitStack();

    UnitStack getUnit() const ;

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::DialogAddUnitStack *ui;
};

#endif // DIALOGADDUNITSTACK_H
