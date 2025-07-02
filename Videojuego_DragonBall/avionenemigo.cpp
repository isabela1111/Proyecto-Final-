#include "avionenemigo.h"
#include "recursos.h"
#include "gokunube.h"
#include <QGraphicsScene>

AvionEnemigo::AvionEnemigo(GokuNube* goku, QGraphicsItem* parent)
    : QObject(), QGraphicsPixmapItem(parent),
    frameActual(0), totalFrames(5),
    spriteAncho(228 / 5), spriteAlto(43), goku(goku){
    if (rand() % 2 == 0)
        hojaSprites.load(Recursos::avionEnemigo1Sprite);
    else
        hojaSprites.load(Recursos::avionEnemigo2Sprite);

    setPixmap(hojaSprites.copy(0, 0, spriteAncho, spriteAlto));
    setZValue(1);

    timerMovimiento = new QTimer(this);
    connect(timerMovimiento, &QTimer::timeout, this, &AvionEnemigo::mover);
    timerMovimiento->start(30);

    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, &AvionEnemigo::animar);
    timerAnimacion->start(150);
}

void AvionEnemigo::mover() {
    setX(x() - 4);
    if (!scene() || !goku || !goku->scene()) {
        deleteLater();
        return;
    }
    if (collidesWithItem(goku)) {
        emit colisionaConGoku();
        if (scene())
            scene()->removeItem(this);
        deleteLater();
        return;
    }
    if (x() + boundingRect().width() < 0) {
        if (scene())
            scene()->removeItem(this);
        deleteLater();
    }
}

void AvionEnemigo::animar() {
    int x = (frameActual % totalFrames) * spriteAncho;
    setPixmap(hojaSprites.copy(x, 0, spriteAncho, spriteAlto));
    frameActual++;
}
