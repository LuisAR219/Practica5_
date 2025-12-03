/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "gaficawidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    GraficaWidget *grafica;
    QSlider *pasoSlider;
    QPushButton *abrirButton;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        grafica = new GraficaWidget(centralwidget);
        grafica->setObjectName("grafica");

        verticalLayout->addWidget(grafica);

        pasoSlider = new QSlider(centralwidget);
        pasoSlider->setObjectName("pasoSlider");
        pasoSlider->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(pasoSlider);

        abrirButton = new QPushButton(centralwidget);
        abrirButton->setObjectName("abrirButton");

        verticalLayout->addWidget(abrirButton);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Gr\303\241fica Simulaci\303\263n", nullptr));
        abrirButton->setText(QCoreApplication::translate("MainWindow", "Abrir simulacion.txt", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
