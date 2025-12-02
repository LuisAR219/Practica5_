#ifndef SIMULADOR_H
#define SIMULADOR_H

#include <vector>
#include <fstream>
#include "particula.h"
#include "obstaculo.h"

class Simulador
{
public:
    Simulador(double anchoCaja, double altoCaja, double dt);
    void agregarParticula(const Particula& p);
    void agregarObstaculo(const Obstaculo& o);
    void ejecutar(int pasos);

private:
    std::vector<Particula> particulas;
    std::vector<Obstaculo> obstaculos;
    double anchoCaja;
    double altoCaja;
    double dt;
    std::ofstream archivoSalida;

    void verificarColisionesParedes(int step);
    void verificarColisionesObstaculos(int step);
    void verificarColisionesParticulas(int step);

    bool colisionCirculoRectangulo(const Particula& p, const Rectangulo& r);
    Vector2D obtenerNormalRectangulo(const Particula& p, const Rectangulo& r);
};

#endif
