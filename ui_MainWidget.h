/********************************************************************************
** Form generated from reading UI file 'MainWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWIDGET_H
#define UI_MAINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWidget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *topLayout;
    QWidget *subWidget1;
    QWidget *subWidget2;
    QWidget *menuWidgetContainer;

    void setupUi(QWidget *MainWidget)
    {
        if (MainWidget->objectName().isEmpty())
            MainWidget->setObjectName(QString::fromUtf8("MainWidget"));
        MainWidget->resize(800, 600);
        verticalLayout = new QVBoxLayout(MainWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        topLayout = new QHBoxLayout();
        topLayout->setObjectName(QString::fromUtf8("topLayout"));
        subWidget1 = new QWidget(MainWidget);
        subWidget1->setObjectName(QString::fromUtf8("subWidget1"));
        subWidget1->setMinimumSize(QSize(300, 300));
        subWidget1->setStyleSheet(QString::fromUtf8("border: 2px solid red;"));

        topLayout->addWidget(subWidget1);

        subWidget2 = new QWidget(MainWidget);
        subWidget2->setObjectName(QString::fromUtf8("subWidget2"));
        subWidget2->setMinimumSize(QSize(300, 300));
        subWidget2->setStyleSheet(QString::fromUtf8("border: 2px solid blue;"));

        topLayout->addWidget(subWidget2);


        verticalLayout->addLayout(topLayout);

        menuWidgetContainer = new QWidget(MainWidget);
        menuWidgetContainer->setObjectName(QString::fromUtf8("menuWidgetContainer"));
        menuWidgetContainer->setMinimumSize(QSize(0, 200));
        menuWidgetContainer->setStyleSheet(QString::fromUtf8("border: 2px solid green;"));

        verticalLayout->addWidget(menuWidgetContainer);


        retranslateUi(MainWidget);

        QMetaObject::connectSlotsByName(MainWidget);
    } // setupUi

    void retranslateUi(QWidget *MainWidget)
    {
        MainWidget->setWindowTitle(QCoreApplication::translate("MainWidget", "MainWidget", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWidget: public Ui_MainWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWIDGET_H
