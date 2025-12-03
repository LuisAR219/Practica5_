#include "simulador.h"
#include <cmath>
#include <algorithm>

Simulador::Simulador(double ancho, double alto, double deltaT)
    : anchoCaja(ancho), altoCaja(alto), dt(deltaT)
{
    archivoSalida.open("simulacion.txt");
}

void Simulador::agregarParticula(const Particula& p)
{
    particulas.push_back(p);
}

void Simulador::agregarObstaculo(const Obstaculo& o)
{
    obstaculos.push_back(o);
}

void Simulador::ejecutar(int pasos)
{
    for (int step = 0; step < pasos; ++step)
    {
        // Actualizar posiciones
        for (auto& p : particulas)
            p.actualizar(dt);

        // Detectar y resolver colisiones
        verificarColisionesParedes(step);
        verificarColisionesObstaculos(step);
        verificarColisionesParticulas(step);

        // Guardar posiciones
        archivoSalida << "Paso " << step << "\n";
        for (const auto& p : particulas)
            archivoSalida << p.pos.x << " " << p.pos.y << "\n";
    }
    archivoSalida.close();
}

void Simulador::verificarColisionesParedes(int step)
{
    for (auto &p : particulas)
    {
        bool colision = false;

        // Guardamos coordenadas LÓGICAS antes de mover
        double xAnt = p.pos.x;
        double yAnt = p.pos.y;

        // Detección y corrección
        if (p.pos.x - p.radio < 0.0) {
            p.pos.x = p.radio;
            p.vel.x *= -1;
            colision = true;
        } else if (p.pos.x + p.radio > anchoCaja) {
            p.pos.x = anchoCaja - p.radio;
            p.vel.x *= -1;
            colision = true;
        }

        if (p.pos.y - p.radio < 0.0) {
            p.pos.y = p.radio;
            p.vel.y *= -1;
            colision = true;
        } else if (p.pos.y + p.radio > altoCaja) {
            p.pos.y = altoCaja - p.radio;
            p.vel.y *= -1;
            colision = true;
        }

        if (colision)
            archivoSalida << "COLISION PARED " << step
                          << " " << xAnt << " " << yAnt << '\n';
    }
}

bool Simulador::colisionCirculoRectangulo(const Particula& p, const Rectangulo& r)
{
    double cx = std::max(r.x, std::min(p.pos.x, r.x + r.ancho));
    double cy = std::max(r.y, std::min(p.pos.y, r.y + r.alto));
    double dx = p.pos.x - cx;
    double dy = p.pos.y - cy;
    return (dx * dx + dy * dy) <= (p.radio * p.radio);
}

Vector2D Simulador::obtenerNormalRectangulo(const Particula& p, const Rectangulo& r)
{
    double cntX = r.x + r.ancho / 2.0;
    double cntY = r.y + r.alto / 2.0;
    double dx = p.pos.x - cntX;
    double dy = p.pos.y - cntY;
    if (std::abs(dx) > std::abs(dy))
        return { dx > 0 ? 1.0 : -1.0, 0.0 };
    else
        return { 0.0, dy > 0 ? 1.0 : -1.0 };
}

void Simulador::verificarColisionesObstaculos(int step)
{
    for (auto &p : particulas)
    {
        for (const auto &obs : obstaculos)
        {
            if (colisionCirculoRectangulo(p, obs.rect))
            {
                Vector2D normal = obtenerNormalRectangulo(p, obs.rect);
                double vNormal = p.vel.x * normal.x + p.vel.y * normal.y;

                if (vNormal < 0) // solo impacto entrante
                {
                    double xCol = p.pos.x;
                    double yCol = p.pos.y;
                    archivoSalida << "COLISION OBST " << step
                                  << " " << xCol << " " << yCol << '\n';

                    // Aplicar restitución
                    p.vel.x -= (1 + obs.coefRestitucion) * vNormal * normal.x;
                    p.vel.y -= (1 + obs.coefRestitucion) * vNormal * normal.y;
                }
            }
        }
    }
}

void Simulador::verificarColisionesParticulas(int step)
{
    for (size_t i = 0; i < particulas.size(); ++i)
    {
        for (size_t j = i + 1; j < particulas.size(); ++j)
        {
            Particula &p1 = particulas[i];
            Particula &p2 = particulas[j];

            double dx = p1.pos.x - p2.pos.x;
            double dy = p1.pos.y - p2.pos.y;
            double dist = std::sqrt(dx * dx + dy * dy);

            if (dist < p1.radio + p2.radio)
            {
                double masaTotal = p1.masa + p2.masa;

                double vx = (p1.masa * p1.vel.x + p2.masa * p2.vel.x) / masaTotal;
                double vy = (p1.masa * p1.vel.y + p2.masa * p2.vel.y) / masaTotal;

                // Centro de masa (ANTES de modificar)
                double newX = (p1.masa * p1.pos.x + p2.masa * p2.pos.x) / masaTotal;
                double newY = (p1.masa * p1.pos.y + p2.masa * p2.pos.y) / masaTotal;

                archivoSalida << "COLISION PP " << step
                              << " " << newX << " " << newY << '\n';

                // Fusionar en p1
                p1.pos.x = newX;
                p1.pos.y = newY;
                p1.vel.x = vx;
                p1.vel.y = vy;
                p1.masa = masaTotal;
                p1.radio = std::sqrt(p1.radio * p1.radio + p2.radio * p2.radio);

                // Eliminar p2
                particulas.erase(particulas.begin() + j);
                --j;
            }
        }
    }
}
