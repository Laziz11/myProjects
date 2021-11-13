/********************************************************************************
** Form generated from reading UI file 'dialogaddobjectstack.ui'
**
** Created by: Qt User Interface Compiler version 6.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGADDOBJECTSTACK_H
#define UI_DIALOGADDOBJECTSTACK_H

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

class Ui_DialogAddObjectStack
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *lineName;
    QLabel *label_2;
    QLineEdit *linePrice;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DialogAddObjectStack)
    {
        if (DialogAddObjectStack->objectName().isEmpty())
            DialogAddObjectStack->setObjectName(QString::fromUtf8("DialogAddObjectStack"));
        DialogAddObjectStack->resize(291, 131);
        verticalLayout = new QVBoxLayout(DialogAddObjectStack);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        widget = new QWidget(DialogAddObjectStack);
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


        verticalLayout->addWidget(widget);

        buttonBox = new QDialogButtonBox(DialogAddObjectStack);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setLayoutDirection(Qt::LeftToRight);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(DialogAddObjectStack);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, DialogAddObjectStack, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, DialogAddObjectStack, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(DialogAddObjectStack);
    } // setupUi

    void retranslateUi(QDialog *DialogAddObjectStack)
    {
        DialogAddObjectStack->setWindowTitle(QCoreApplication::translate("DialogAddObjectStack", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("DialogAddObjectStack", "\320\230\320\274\321\217", nullptr));
        label_2->setText(QCoreApplication::translate("DialogAddObjectStack", "\320\246\320\265\320\275\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogAddObjectStack: public Ui_DialogAddObjectStack {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGADDOBJECTSTACK_H
