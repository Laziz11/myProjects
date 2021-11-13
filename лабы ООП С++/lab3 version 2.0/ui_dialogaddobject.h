/********************************************************************************
** Form generated from reading UI file 'dialogaddobject.ui'
**
** Created by: Qt User Interface Compiler version 6.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGADDOBJECT_H
#define UI_DIALOGADDOBJECT_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DialogAddObject
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *lineName;
    QLabel *label_2;
    QLineEdit *linePrice;
    QLabel *label_3;
    QLineEdit *lineVolume;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DialogAddObject)
    {
        if (DialogAddObject->objectName().isEmpty())
            DialogAddObject->setObjectName(QString::fromUtf8("DialogAddObject"));
        DialogAddObject->resize(356, 162);
        verticalLayout = new QVBoxLayout(DialogAddObject);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        widget = new QWidget(DialogAddObject);
        widget->setObjectName(QString::fromUtf8("widget"));
        formLayout = new QFormLayout(widget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        lineName = new QLineEdit(widget);
        lineName->setObjectName(QString::fromUtf8("lineName"));

        formLayout->setWidget(0, QFormLayout::FieldRole, lineName);

        label_2 = new QLabel(widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        linePrice = new QLineEdit(widget);
        linePrice->setObjectName(QString::fromUtf8("linePrice"));

        formLayout->setWidget(1, QFormLayout::FieldRole, linePrice);

        label_3 = new QLabel(widget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        lineVolume = new QLineEdit(widget);
        lineVolume->setObjectName(QString::fromUtf8("lineVolume"));

        formLayout->setWidget(2, QFormLayout::FieldRole, lineVolume);


        verticalLayout->addWidget(widget);

        buttonBox = new QDialogButtonBox(DialogAddObject);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setLocale(QLocale(QLocale::C, QLocale::AnyCountry));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(DialogAddObject);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, DialogAddObject, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, DialogAddObject, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(DialogAddObject);
    } // setupUi

    void retranslateUi(QDialog *DialogAddObject)
    {
        DialogAddObject->setWindowTitle(QCoreApplication::translate("DialogAddObject", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("DialogAddObject", "Name", nullptr));
        label_2->setText(QCoreApplication::translate("DialogAddObject", "Price", nullptr));
        label_3->setText(QCoreApplication::translate("DialogAddObject", "Volume", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogAddObject: public Ui_DialogAddObject {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGADDOBJECT_H
