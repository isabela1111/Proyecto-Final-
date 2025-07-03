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

signals:
    void granadaLanzada(Granada* granada);

private:
    Personaje* objetivoJugador;
    bool puedeAtacar;
    QTimer* movimientoTimer;
};

#endif // TAOPAIPAIJEFE_H
