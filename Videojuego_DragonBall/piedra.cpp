#include "piedra.h"
#include "recursos.h"
#include <QGraphicsScene>

Piedra::Piedra(Nivel1* nivel, QGraphicsItem* parent)
    : Obstaculo(nivel, parent),
    frameActual(0), totalFrames(8),
    anchoFrame(81), altoFrame(79)
{
    hojaSprites.load(Recursos::RocaSprite);
    setZValue(1);
    setPos(285, 0);
    actualizarFrame();
    // Animación del sprite
    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, [=]() {
        frameActual = (frameActual + 1) % totalFrames;
        actualizarFrame();});
    timerAnimacion->start(100);
}

void Piedra::actualizarFrame(){
    int fila = 1;
    QPixmap frame = hojaSprites.copy(
        frameActual * anchoFrame,
        fila * altoFrame,
        anchoFrame,
        altoFrame );
    setPixmap(frame.scaled(40, 40));
}
