#ifndef OBSTACULO_H
#define OBSTACULO_H

struct Rectangulo {
    double x, y, ancho, alto;
    Rectangulo(double x, double y, double w, double h) : x(x), y(y), ancho(w), alto(h) {}
};

class Obstaculo {
public:
    Rectangulo rect;
    double coefRestitucion;

    Obstaculo(double x, double y, double w, double h, double e);
};

#endif
