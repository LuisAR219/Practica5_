#pragma once
#include <QWidget>
#include <vector>
#include <QPointF>

struct Rectangulo {
    double x, y, ancho, alto;
};

class GraficaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GraficaWidget(QWidget *parent = nullptr);

    void cargarArchivo(const QString &archivo);
    void setPaso(int p);
    void setObstaculos(const std::vector<Rectangulo>& obs);

    int pasoMax() const { return m_pasoMax; }
    int pasoAct() const { return m_pasoAct; }

protected:
    void paintEvent(QPaintEvent *) override;

private:
    // Trayectorias por partícula: m_tray[part][step]
    std::vector<std::vector<QPointF>> m_tray;

    // Colisiones por paso: m_colsByStep[step]
    std::vector<std::vector<QPointF>> m_colsByStep;

    // Obstáculos
    std::vector<Rectangulo> m_obstaculos;

    int m_pasoMax = 0;
    int m_pasoAct = 0;
};

