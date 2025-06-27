#include "taopaipaijefe.h"
#include "recursos.h"

#include <QTimer>
#include <QDebug>

TaoPaiPaiJefe::TaoPaiPaiJefe(QGraphicsView* vista, QObject* parent)
    : Personaje(vista, parent), puedeAtacar(true) {
    nombre = "Tao Pai Pai";
    vida = 5;
    velocidad = 3;

    hojaSprites.load(Recursos::TaoenemigoSprite);
    spriteX = 0;
    spriteY = 0;
    spriteAncho = 60;
    spriteAlto = 60;

    sprite = hojaSprites.copy(spriteX, spriteY, spriteAncho, spriteAlto);
    setPixmap(sprite);
}

void TaoPaiPaiJefe::atacar() {
    lanzarGranada();
}

void TaoPaiPaiJefe::lanzarGranada() {
    if (!puedeAtacar || !objetivoJugador) return;

    qDebug() << "Tao Pai Pai lanza una granada.";

    Granada* granada = new Granada(x(), y(), objetivoJugador);
    emit granadaLanzada(granada);

    puedeAtacar = false;
    QTimer::singleShot(1500, this, [this]() {
        puedeAtacar = true;
    });
}

void TaoPaiPaiJefe::setObjetivo(Personaje* objetivo) {
    objetivoJugador = objetivo;
}

