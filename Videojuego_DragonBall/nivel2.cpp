#include "nivel2.h"
#include <QGraphicsScene>
#include <QTimer>
#include <QDebug>

Nivel2::Nivel2(QObject* parent) : Nivel(parent) {

    fondo.load(":/fondo_nivel2.png");
    QGraphicsPixmapItem* fondoItem = new QGraphicsPixmapItem(fondo);
    escena->addItem(fondoItem);

    goku = new Goku();
    taoPaiPai = new TaoPaiPai(vista, this);


    goku->setPos(100, 400);
    taoPaiPai->setPos(600, 400);

    escena->addItem(goku);
    escena->addItem(taoPaiPai);

    personajes.append(goku);
    personajes.append(taoPaiPai);

    QTimer* timerAtaques = new QTimer(this);
    connect(timerAtaques, &QTimer::timeout, this, &Nivel2::gestionarAtaques);
    timerAtaques->start(2500); // Cada 2.5 segundos
}

void Nivel2::gestionarAtaques() {
    // Tao Pai Pai lanza granada de forma parabolica

}
