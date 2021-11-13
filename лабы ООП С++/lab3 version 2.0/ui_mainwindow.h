/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *butAddObject;
    QPushButton *butAddObjectStack;
    QPushButton *butDeleteAll;
    QPushButton *butDelItem;
    QPushButton *butSave;
    QPushButton *butLoad;
    QSpacerItem *horizontalSpacer;
    QListWidget *listObjects;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        widget = new QWidget(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setMinimumSize(QSize(0, 40));
        widget->setMaximumSize(QSize(16777215, 40));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        butAddObject = new QPushButton(widget);
        butAddObject->setObjectName(QString::fromUtf8("butAddObject"));

        horizontalLayout->addWidget(butAddObject);

        butAddObjectStack = new QPushButton(widget);
        butAddObjectStack->setObjectName(QString::fromUtf8("butAddObjectStack"));

        horizontalLayout->addWidget(butAddObjectStack);

        butDeleteAll = new QPushButton(widget);
        butDeleteAll->setObjectName(QString::fromUtf8("butDeleteAll"));

        horizontalLayout->addWidget(butDeleteAll);

        butDelItem = new QPushButton(widget);
        butDelItem->setObjectName(QString::fromUtf8("butDelItem"));

        horizontalLayout->addWidget(butDelItem);

        butSave = new QPushButton(widget);
        butSave->setObjectName(QString::fromUtf8("butSave"));

        horizontalLayout->addWidget(butSave);

        butLoad = new QPushButton(widget);
        butLoad->setObjectName(QString::fromUtf8("butLoad"));

        horizontalLayout->addWidget(butLoad);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addWidget(widget);

        listObjects = new QListWidget(centralwidget);
        listObjects->setObjectName(QString::fromUtf8("listObjects"));

        verticalLayout->addWidget(listObjects);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\320\232\320\276\320\273\320\273\320\265\320\272\321\206\320\270\321\217 \320\272\320\273\320\260\321\201\321\201\320\276\320\262", nullptr));
        butAddObject->setText(QCoreApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \320\262\320\265\321\211\321\214", nullptr));
        butAddObjectStack->setText(QCoreApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \321\201\321\202\320\276\320\277\320\272\321\203 \320\274\320\276\320\275\320\265\321\202", nullptr));
        butDeleteAll->setText(QCoreApplication::translate("MainWindow", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \320\262\321\201\320\265", nullptr));
        butDelItem->setText(QCoreApplication::translate("MainWindow", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214", nullptr));
        butSave->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\277\320\270\321\201\321\214 \320\262 \321\204\320\260\320\271\320\273", nullptr));
        butLoad->setText(QCoreApplication::translate("MainWindow", "\320\247\321\202\320\265\320\275\320\270\320\265 \320\270\320\267 \321\204\320\260\320\271\320\273\320\276\320\262.", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
