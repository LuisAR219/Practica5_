#include "obstaculo.h"

Obstaculo::Obstaculo(double x, double y, double w, double h, double e)
    : rect(x, y, w, h), coefRestitucion(e) {}
