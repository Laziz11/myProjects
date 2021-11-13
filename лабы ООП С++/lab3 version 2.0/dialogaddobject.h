#ifndef DIALOGADDOBJECT_H
#define DIALOGADDOBJECT_H

#include <QDialog>
#include "object.h"

namespace Ui {
class DialogAddObject;
}

class DialogAddObject : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddObject(QWidget *parent = nullptr);
    ~DialogAddObject();

    Object getObject() const ;

private slots:
    void onClickAccept();

    void onClickCancel();

    void on_lineName_textChanged(const QString &arg1);

    void on_linePrice_textChanged(const QString &arg1);

    void on_lineVolume_textChanged(const QString &arg1);

private:
    Ui::DialogAddObject *ui;
};

#endif // DIALOGADDOBJECT_H
