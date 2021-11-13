/********************************************************************************
** Form generated from reading UI file 'dialogisneedtosaveold.ui'
**
** Created by: Qt User Interface Compiler version 6.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGISNEEDTOSAVEOLD_H
#define UI_DIALOGISNEEDTOSAVEOLD_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DialogIsneedToSaveOld
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DialogIsneedToSaveOld)
    {
        if (DialogIsneedToSaveOld->objectName().isEmpty())
            DialogIsneedToSaveOld->setObjectName(QString::fromUtf8("DialogIsneedToSaveOld"));
        DialogIsneedToSaveOld->resize(400, 300);
        verticalLayout = new QVBoxLayout(DialogIsneedToSaveOld);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(DialogIsneedToSaveOld);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        buttonBox = new QDialogButtonBox(DialogIsneedToSaveOld);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(DialogIsneedToSaveOld);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, DialogIsneedToSaveOld, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, DialogIsneedToSaveOld, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(DialogIsneedToSaveOld);
    } // setupUi

    void retranslateUi(QDialog *DialogIsneedToSaveOld)
    {
        DialogIsneedToSaveOld->setWindowTitle(QCoreApplication::translate("DialogIsneedToSaveOld", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("DialogIsneedToSaveOld", "Do you want to save old list?", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogIsneedToSaveOld: public Ui_DialogIsneedToSaveOld {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGISNEEDTOSAVEOLD_H
