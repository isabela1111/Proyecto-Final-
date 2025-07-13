#ifndef GOKU_H
#define GOKU_H

#include "personaje.h"
#include <QTimer>

class Goku : public Personaje {
    Q_OBJECT

public:
    Goku(QGraphicsView* vista = nullptr, QObject* parent = nullptr);

    void mover() override {}
    void saltar() override;
    void atacar() override;
    void recibirDanio(int cantidad) override;
    void devolverGranada();
    bool estaEnModoAtaque() const;
    void keyPressEvent(QKeyEvent* event) override;

    void moverIzquierda();
    void moverDerecha();
    void mostrarCaida();
    void actualizarSalto();
    enum Direccion { IZQUIERDA, DERECHA };

private:
    Direccion direccionActual;



signals:
    void danioRecibido();

private:
    int frameActual;
    int tiempoSalto;
    bool enElAire;
    bool modoAtaque;
    bool ataqueAlterno;

    QTimer* temporizadorAtaque;
    QTimer* temporizadorSalto;

};

#endif // GOKU_H
