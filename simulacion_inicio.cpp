#include "simulacion_inicio.h"
#include "simulador.h"
#include "particula.h"
#include "obstaculo.h"
#include <iostream>

void ejecutarSimulacion()
{
    std::cout << "[Simulación] Iniciando...\n";

    Simulador sim(800, 600, 0.01);   // ancho, alto, dt

    // Partículas de ejemplo
    sim.agregarParticula(Particula(100, 100, 50, 30, 1.0, 10));
    sim.agregarParticula(Particula(150, 200, 120, -20, 1.0, 10));
    sim.agregarParticula(Particula(650, 220, -150, 0, 2.0, 12));

    // Obstáculos
    sim.agregarObstaculo(Obstaculo(400, 300, 50, 50, 0.5));
    sim.agregarObstaculo(Obstaculo(200, 400, 60, 40, 0.3));
    sim.agregarObstaculo(Obstaculo(500, 200, 40, 60, 0.7));
    sim.agregarObstaculo(Obstaculo(100, 500, 80, 30, 0.4));

    // Ejecuta 1000 pasos
    sim.ejecutar(1000);

    std::cout << "[Simulación] Terminada → simulacion.txt generado\n";
}
