#ifndef DIALOGISNEEDTOSAVEOLD_H
#define DIALOGISNEEDTOSAVEOLD_H

#include <QDialog>

namespace Ui {
class DialogIsneedToSaveOld;
}

class DialogIsneedToSaveOld : public QDialog
{
    Q_OBJECT

public:
    explicit DialogIsneedToSaveOld(QWidget *parent = nullptr);
    ~DialogIsneedToSaveOld();

private:
    Ui::DialogIsneedToSaveOld *ui;
private slots:
    void onClickAccept();

    void onClickCancel();
};

#endif // DIALOGISNEEDTOSAVEOLD_H
