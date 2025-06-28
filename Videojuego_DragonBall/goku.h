#ifndef GOKU_H
#define GOKU_H

#include "personaje.h"

class Goku : public Personaje {
    Q_OBJECT

public:
    Goku(QGraphicsView* vista = nullptr, QObject* parent = nullptr);

    void mover() override;
    void saltar() override;
    void atacar() override;
    void usarNube();
    void devolverGranada();  // activa modo ataque

    void keyPressEvent(QKeyEvent* event) override;

    bool estaEnModoAtaque() const;

private:
    int frameActual;
    int tiempoSalto;
    bool enElAire;
    bool modoAtaque;
    QTimer* temporizadorAtaque;
};

#endif // GOKU_H
