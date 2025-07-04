#include "misil.h"
#include "recursos.h"
#include "gokunube.h"
#include <QGraphicsScene>

Misil::Misil(GokuNube* goku, QGraphicsItem* parent)
    : Obstaculo(nullptr, parent), goku(goku), frameActual(0)
{
    sprite.load(Recursos::misilSprite);
    setPixmap(sprite.copy(0, 0, 34, 20));  // 102 / 3 = 34 px por frame
    setZValue(2);
    setShapeMode(QGraphicsPixmapItem::BoundingRectShape);

    timerMovimiento = new QTimer(this);
    connect(timerMovimiento, &QTimer::timeout, this, &Misil::mover);
    timerMovimiento->start(30);

    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, [=]() {
        frameActual = (frameActual + 1) % 3;
        setPixmap(sprite.copy(frameActual * 34, 0, 34, 20));
    });
    timerAnimacion->start(150);
}

void Misil::mover() {
    setX(x() - 4);
    if (!scene() || !goku || !goku->scene()) {
        limpiarYEliminar();
        return;
    }
    QList<QGraphicsItem*> colisiones = collidingItems();
    for (QGraphicsItem* item : colisiones) {
        GokuNube* gokuColisionado = dynamic_cast<GokuNube*>(item);
        if (gokuColisionado) {
            gokuColisionado->recibirDanio(1);
            if (scene()) scene()->removeItem(this);
            limpiarYEliminar();
            return;
        }
    }
    if (x() + boundingRect().width() < 0) {
        if (scene()) scene()->removeItem(this);
        limpiarYEliminar();
    }
}

void Misil::limpiarYEliminar() {
    if (timerMovimiento) {
        timerMovimiento->stop();
        timerMovimiento->deleteLater();
        timerMovimiento = nullptr;
    }
    if (timerAnimacion) {
        timerAnimacion->stop();
        timerAnimacion->deleteLater();
        timerAnimacion = nullptr;
    }
    deleteLater();
}

