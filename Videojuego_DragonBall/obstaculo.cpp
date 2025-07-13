#include "obstaculo.h"
#include "nivel1.h"
#include "taopaipai.h"

Obstaculo::Obstaculo(Nivel1* nivel, QGraphicsItem* parent)
    : QObject(), QGraphicsPixmapItem(parent), nivel(nivel){
    timerMovimiento = new QTimer(this);
    connect(timerMovimiento, &QTimer::timeout, this, [=]() {
        mover();
        verificarColision();
    });
    timerMovimiento->start(30);
}

void Obstaculo::mover(){
    moveBy(0, 5);  // Baja 5 píxeles
    if (y() > 600) {
        scene()->removeItem(this);
        deleteLater();
    }
}

void Obstaculo::verificarColision(){
    QList<QGraphicsItem*> colisiones = collidingItems();
    for (QGraphicsItem* item : colisiones) {
        if (dynamic_cast<TaoPaiPai*>(item)) {
            nivel->quitarVida();
            scene()->removeItem(this);
            deleteLater();
            break;
        }
    }
}
