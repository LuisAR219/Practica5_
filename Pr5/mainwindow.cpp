#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gaficawidget.h"
#include <QFileDialog>
#include <QPushButton>
#include <QSlider>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->abrirButton,  &QPushButton::clicked,
            this, &MainWindow::on_abrirClicked);

    connect(ui->pasoSlider,   &QSlider::valueChanged,
            this, &MainWindow::on_sliderChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_abrirClicked()
{
    QString file =
        QFileDialog::getOpenFileName(this, "Abrir simulacion.txt", "", "*.txt");

    if (file.isEmpty())
        return;

    auto *g = ui->grafica;

    // 1. Cargar archivo con trayectorias y colisiones
    g->cargarArchivo(file);

    // 2. Enviar obstáculos (los mismos del simulador)
    std::vector<Rectangulo> obs = {
        {400, 300, 50, 50},
        {200, 400, 60, 40},
        {500, 200, 40, 60},
        {100, 500, 80, 30}
    };

    g->setObstaculos(obs);

    // Configurar slider
    ui->pasoSlider->setMaximum(g->pasoMax());
    ui->pasoSlider->setValue(g->pasoAct());
}

void MainWindow::on_sliderChanged(int v)
{
    ui->grafica->setPaso(v);
}
