#ifndef TAOPAIPAIJEFE_H
#define TAOPAIPAIJEFE_H

#include "personaje.h"
#include "granada.h"
#include <QTimer>

class TaoPaiPaiJefe : public Personaje {
    Q_OBJECT

public:
    TaoPaiPaiJefe(QGraphicsView* vista = nullptr, QObject* parent = nullptr);
    void atacar() override;
    void lanzarGranada();
    void setObjetivo(Personaje* objetivo);
    void mover() override;
    void recibirDanio(int cantidad);

signals:
    void granadaLanzada(Granada* granada);

private:
    Personaje* objetivoJugador;
    bool puedeAtacar;

    int direccion;
    int filaActualAtaque;
    int columnaAtaque;
    QTimer* movimientoTimer;
    QTimer* ataqueTimer;
    int frameMovimiento;
    int frameAtaque;
    bool estaCayendo;

    QPixmap hojaMovimiento;


};

#endif // TAOPAIPAIJEFE_H
