#ifndef GRANADA_H
#define GRANADA_H

#include "obstaculo.h"

#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QObject>

class Granada : public Obstaculo {
    Q_OBJECT

public:
    Granada(qreal xInicial, qreal yInicial, QGraphicsItem* objetivoJugador = nullptr, bool esDevuelta = false);

    void mover() override;

private:
    void explotar();

    QPixmap hojaGranada;
    QPixmap explosionSprite;

    QGraphicsItem* objetivo;

    int spriteAncho;
    int spriteAlto;
    int frameActual;
    int frameExplosion;
    bool explotando;
    bool devuelta;
    bool yaColisiono;


    qreal velocidadX;
    qreal velocidadY;
    qreal gravedad;
};

#endif // GRANADA_H
