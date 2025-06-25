#include "taopaipaijefe.h"
#include <QTimer>
#include <QDebug>

TaoPaiPaiJefe::TaoPaiPaiJefe(QGraphicsView* vista, QObject* parent)
    : Personaje(vista, parent), puedeAtacar(true) {
    nombre = "Tao Pai Pai";
    vida = 5;
    velocidad = 3;

    hojaSprites.load(":/Recursos/Sprites/taopaipaienemie.png");
    spriteX = 0;
    spriteY = 0;
    spriteAncho = 60;
    spriteAlto = 60;

    sprite = hojaSprites.copy(spriteX, spriteY, spriteAncho, spriteAlto);
    setPixmap(sprite);
}

void TaoPaiPaiJefe::atacar() {
    // Se lanza granada por temporizador
    lanzarGranada();
}

void TaoPaiPaiJefe::lanzarGranada() {
    if (!puedeAtacar) return;

    qDebug() << "Tao Pai Pai lanza una granada.";

    Granada* granada = new Granada(x(), y(), objetivoJugador);
    emit granadaLanzada(granada);

    puedeAtacar = false;
    QTimer::singleShot(1500, this, [this]() {
        puedeAtacar = true;
    });
}
