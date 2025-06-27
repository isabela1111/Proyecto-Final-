#include "taopaipaijefe.h"
#include "recursos.h"

#include <QGraphicsScene>
#include <QDebug>

TaoPaiPaiJefe::TaoPaiPaiJefe(QGraphicsView* vista, QObject* parent)
    : Personaje(vista, parent), puedeAtacar(true), objetivoJugador(nullptr) {

    nombre = "Tao Pai Pai";
    vida = 5;
    velocidad = 3;

    hojaSprites.load(Recursos::TaoenemigoSprite);
    spriteX = 0;
    spriteY = 0;
    spriteAncho = 60;
    spriteAlto = 60;

    setPixmap(hojaSprites.copy(spriteX, spriteY, spriteAncho, spriteAlto));
    setPos(600, 400);

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    // Ataques periódicos
    QTimer* ataqueTimer = new QTimer(this);
    connect(ataqueTimer, &QTimer::timeout, this, &TaoPaiPaiJefe::atacar);
    ataqueTimer->start(2500); // Cada 2.5 segundos

    // Movimiento automático
    movimientoTimer = new QTimer(this);
    connect(movimientoTimer, &QTimer::timeout, this, &TaoPaiPaiJefe::mover);
    movimientoTimer->start(1800);
}

void TaoPaiPaiJefe::atacar() {
    lanzarGranada();
}

void TaoPaiPaiJefe::lanzarGranada() {
    if (!puedeAtacar || !objetivoJugador) return;

    qDebug() << "Tao Pai Pai lanza una granada.";

    Granada* granada = new Granada(x(), y(), objetivoJugador);
    emit granadaLanzada(granada);

    if (scene()) scene()->addItem(granada);

    puedeAtacar = false;
    QTimer::singleShot(1500, this, [this]() {
        puedeAtacar = true;
    });
}

void TaoPaiPaiJefe::setObjetivo(Personaje* objetivo) {
    objetivoJugador = objetivo;
}

void TaoPaiPaiJefe::mover() {
    // Movimiento simple lateral hacia Goku (aleatorio o alternante si se prefiere)
    if (!objetivoJugador) return;

    if (x() > objetivoJugador->x()) {
        moveBy(-velocidad, 0);
    } else if (x() < objetivoJugador->x()) {
        moveBy(velocidad, 0);
    }
    
    spriteX = (spriteX + 1) % 4;
    setPixmap(hojaSprites.copy(spriteX * spriteAncho, 0, spriteAncho, spriteAlto));
}


