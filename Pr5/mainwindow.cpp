#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gaficawidget.h"
#include <QFileDialog>
#include <QPushButton>   // ← faltaba
#include <QSlider>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->abrirButton,  &QPushButton::clicked,
            this, &MainWindow::on_abrirClicked);
    connect(ui->pasoSlider,   &QSlider::valueChanged,
            this, &MainWindow::on_sliderChanged);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_abrirClicked()
{
    QString file =
        QFileDialog::getOpenFileName(this, "Abrir simulacion.txt", "", "*.txt");
    if (file.isEmpty()) return;

    auto *g = ui->grafica;               // widget promocionado
    g->cargarArchivo(file);
    ui->pasoSlider->setMaximum(g->pasoMax());
    ui->pasoSlider->setValue(g->pasoAct());
}

void MainWindow::on_sliderChanged(int v)
{
    ui->grafica->setPaso(v);
}
