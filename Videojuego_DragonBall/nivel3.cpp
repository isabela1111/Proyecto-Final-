#include "nivel3.h"
#include "goku.h"
#include <QGraphicsScene>
#include <QTimer>

Nivel3::Nivel3(QObject* parent) : Nivel(parent) {
    velocidadVuelo = 3.5;
    goku = new Goku();

    fondo.load(":/fondo_nivel3.png");
    QGraphicsPixmapItem* fondoItem = new QGraphicsPixmapItem(fondo);
    escena->addItem(fondoItem);

    goku->setPos(100, 400);
    escena->addItem(goku);
    personajes.append(goku);

    QTimer* enemigoTimer = new QTimer(this);
    connect(enemigoTimer, &QTimer::timeout, this, &Nivel3::generarEnemigos);
    enemigoTimer->start(2000); // Cada 2 segundos
}

void Nivel3::generarEnemigos() {
    // Logica para agregar enemigos al azar
}

void Nivel3::detectarImpactos() {
    // Logica de colisiones
}
void Nivel3::iniciarnivel() {
    qDebug() << "Nivel 3 iniciado (nube voladora hacia la base)";
}

