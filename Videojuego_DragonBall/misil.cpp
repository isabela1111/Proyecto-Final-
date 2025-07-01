#include "misil.h"
#include "gokunube.h"
#include "recursos.h"
#include <QGraphicsScene>

Misil::Misil(GokuNube* goku, QGraphicsItem* parent)
    : QObject(), QGraphicsPixmapItem(parent), goku(goku), velocidad(6)
{
    hojaSprites.load(Recursos::misilSprite);  
    setPixmap(hojaSprites.copy(0, 0, 24, 10)); // Ajusta el tamaño al sprite real
    setZValue(1);

    timerMovimiento = new QTimer(this);
    connect(timerMovimiento, &QTimer::timeout, this, &Misil::mover);
    timerMovimiento->start(30);
}

void Misil::mover() {
    setX(x() - velocidad);

    if (goku && collidesWithItem(goku)) {
        emit colisionaConGoku();
        scene()->removeItem(this);
        deleteLater();
        return;
    }

    if (x() + boundingRect().width() < 0) {
        scene()->removeItem(this);
        deleteLater();
    }
}
