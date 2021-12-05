#ifndef DIALOGNEW_H
#define DIALOGNEW_H

#include <QDialog>

namespace Ui {
class DialogNew;
}

class DialogNew : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNew(QWidget *parent = nullptr);
    ~DialogNew();
    QString getName() const ;
    int getPrice() const ;

private:
    Ui::DialogNew *ui;
};

#endif // DIALOGNEW_H
