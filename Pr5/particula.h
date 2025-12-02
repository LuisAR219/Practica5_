#ifndef PARTICULA_H
#define PARTICULA_H

struct Vector2D {
    double x, y;
    Vector2D(double x = 0, double y = 0) : x(x), y(y) {}
};

class Particula {
public:
    Vector2D pos;
    Vector2D vel;
    double masa;
    double radio;

    Particula(double x, double y, double vx, double vy, double m, double r);
    void actualizar(double dt);
};

#endif
