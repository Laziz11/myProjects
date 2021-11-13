#ifndef DIALOGADDOBJECTSTACK_H
#define DIALOGADDOBJECTSTACK_H

#include <QDialog>
#include "objectstack.h"

namespace Ui {
class DialogAddObjectStack;
}

class DialogAddObjectStack : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddObjectStack(QWidget *parent = nullptr);
    ~DialogAddObjectStack();

    ObjectStack getObject() const ;

private slots:
    void onClickAccept();

    void onClickCancel();

    void on_lineName_textChanged(const QString &arg1);

    void on_linePrice_textChanged(const QString &arg1);

private:
    Ui::DialogAddObjectStack *ui;
};

#endif // DIALOGADDOBJECTSTACK_H
