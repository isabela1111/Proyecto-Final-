#ifndef NIVEL3_H
#define NIVEL3_H

#include "nivel.h"
#include "goku.h"

class Nivel3 : public Nivel {
    Q_OBJECT
public:
    explicit Nivel3(QObject* parent = nullptr);

    float velocidadVuelo;
    Goku* goku;

    void generarEnemigos();
    void detectarImpactos();
    void ajustarDireccion();
};

#endif // NIVEL3_H
