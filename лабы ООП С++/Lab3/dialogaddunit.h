#ifndef DIALOGADDUNIT_H
#define DIALOGADDUNIT_H

#include <QDialog>
#include "unit.h"

namespace Ui {
class DialogAddUnit;
}

class DialogAddUnit : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddUnit(QWidget *parent = nullptr);
    ~DialogAddUnit();

    Unit getUnit() const ;

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::DialogAddUnit *ui;

};

#endif // DIALOGADDUNIT_H
