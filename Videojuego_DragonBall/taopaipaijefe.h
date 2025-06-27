#ifndef TAOPAIPAIJEFE_H
#define TAOPAIPAIJEFE_H

#include "personaje.h"
#include "granada.h"

class TaoPaiPaiJefe : public Personaje {
    Q_OBJECT
public:
    explicit TaoPaiPaiJefe(QGraphicsView* vista = nullptr, QObject* parent = nullptr);
    void atacar() override;
    void lanzarGranada();
    void setObjetivo(Personaje* objetivo);

signals:
    void granadaLanzada(Granada* granada);

private:
    bool puedeAtacar;
    Personaje* objetivoJugador = nullptr;
};

#endif // TAOPAIPAIJEFE_H
