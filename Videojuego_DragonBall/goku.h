#ifndef GOKU_H
#define GOKU_H

#include "personaje.h"

class Goku : public Personaje {
    Q_OBJECT
public:
    explicit Goku(QGraphicsView* vista = nullptr, QObject* parent = nullptr);

    bool tieneNubeVoladora;
    bool modoAtaque;

    void usarNube();
    void devolverGranada();
};

#endif // GOKU_H
