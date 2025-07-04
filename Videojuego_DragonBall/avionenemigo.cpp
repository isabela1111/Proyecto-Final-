#include "avionenemigo.h"
#include "recursos.h"
#include "gokunube.h"
#include "misil.h"

#include <QGraphicsScene>
#include <QRandomGenerator>

AvionEnemigo::AvionEnemigo(GokuNube* goku, QGraphicsItem* parent)
    : Obstaculo(nullptr, parent),
    frameActual(0), totalFrames(5),
    spriteAncho(228 / 5), spriteAlto(43),
    goku(goku), misilesDisparados(0)
{
    if (rand() % 2 == 0)
        hojaSprites.load(Recursos::avionEnemigo1Sprite);
    else
        hojaSprites.load(Recursos::avionEnemigo2Sprite);

    setPixmap(hojaSprites.copy(0, 0, spriteAncho, spriteAlto));
    setZValue(1);

    esEstacionario = QRandomGenerator::global()->bounded(2) == 0;


    timerMovimiento = new QTimer(this);
    connect(timerMovimiento, &QTimer::timeout, this, &AvionEnemigo::mover);
    timerMovimiento->start(30);

    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, &AvionEnemigo::animar);
    timerAnimacion->start(150);

    if (esEstacionario) {
        timerDisparo = new QTimer(this);
        connect(timerDisparo, &QTimer::timeout, this, &AvionEnemigo::disparar);
        timerDisparo->start(800);
    }
}

void AvionEnemigo::mover() {
    if (!scene() || !goku || !goku->scene()) {
        deleteLater();
        return;
    }
    if (!esEstacionario || haTerminadoDisparos)
        setX(x() - 4);
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

void AvionEnemigo::disparar() {
    if (!scene() || misilesDisparados >= maxMisiles) {
        haTerminadoDisparos = true;
        if (timerDisparo) timerDisparo->stop();
        return;
    }
    Misil* misil = new Misil(goku, this);
    misil->setPos(x(), y() + boundingRect().height() / 2 - 10);
    emit disparoMisil(misil);
    misilesDisparados++;
}
