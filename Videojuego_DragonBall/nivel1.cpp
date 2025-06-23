#include "nivel1.h"
#include "taopaipai.h"
#include <QTimer>
#include <QGraphicsScene>

Nivel1::Nivel1(QObject* parent) : Nivel(parent) {

    TaoPaiPai* enemigo = new TaoPaiPai();
    enemigo->setPos(200, 500);
    escena->addItem(enemigo);

    QTimer* timerObstaculos = new QTimer(this);
    connect(timerObstaculos, &QTimer::timeout, this, &Nivel1::generarObstaculos);
    timerObstaculos->start(1500);
}

void Nivel1::generarObstaculos() {

}
