#include "particula.h"

Particula::Particula(double x, double y, double vx, double vy, double m, double r)
    : pos(x, y), vel(vx, vy), masa(m), radio(r) {}

void Particula::actualizar(double dt) {
    pos.x += vel.x * dt;
    pos.y += vel.y * dt;
}
