#include "misil.h"
#include "recursos.h"
#include "gokunube.h"
#include <QGraphicsScene>

Misil::Misil(GokuNube* goku, QGraphicsItem* parent)
    : QObject(), QGraphicsPixmapItem(parent), goku(goku)
{
    sprite.load(Recursos::misilSprite);
    setPixmap(sprite);
    setZValue(2);

    timerMovimiento = new QTimer(this);
    connect(timerMovimiento, &QTimer::timeout, this, &Misil::mover);
    timerMovimiento->start(30);
}

void Misil::mover() {
    setX(x() - 7);

    if (!scene() || !goku || !goku->scene()) {
        deleteLater();
        return;
    }

    if (collidesWithItem(goku)) {
        goku->recibirDanio(1);
        scene()->removeItem(this);
        deleteLater();
        return;
    }

    if (x() + boundingRect().width() < 0) {
        scene()->removeItem(this);
        deleteLater();
    }
}
