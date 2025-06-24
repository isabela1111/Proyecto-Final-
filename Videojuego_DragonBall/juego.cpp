#include "juego.h"
#include "nivel1.h"
#include "nivel2.h"
#include "nivel3.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <QDebug>

QGraphicsScene* scene = nullptr;
QGraphicsView* view = nullptr;

Juego::Juego(QObject* parent) : QObject(parent) {
    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, 800, 600);

    view = new QGraphicsView(scene);
    view->setFixedSize(800, 600);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->show();

    nivelActual = nullptr;
    enEjecucion = false;
}

void Juego::iniciarJuego() {
    enEjecucion = true;
    cambiarNivel(new Nivel1(view));

}

void Juego::cambiarNivel(Nivel* n) {
    if (nivelActual) {
        scene->clear();
        delete nivelActual;
    }

    nivelActual = n;
    nivelActual->iniciarnivel();
    scene->addItem(new QGraphicsPixmapItem(nivelActual->fondo));

    QTimer* actualizador = new QTimer(this);
    connect(actualizador, &QTimer::timeout, this, &Juego::actualizar);
    actualizador->start(16); // 60 fps aprox
}

void Juego::actualizar() {
    if (!enEjecucion || !nivelActual) return;

    nivelActual->actualizar();

    if (nivelActual->terminado) {
        if (dynamic_cast<Nivel1*>(nivelActual)) {
            cambiarNivel(new Nivel2());
        } else if (dynamic_cast<Nivel2*>(nivelActual)) {
            cambiarNivel(new Nivel3());
        } else {
            terminarJuego();
        }
    }
}

void Juego::terminarJuego() {
    enEjecucion = false;
    scene->clear();
    qDebug() << "Juego terminado. ¡Gracias por jugar!";
}
