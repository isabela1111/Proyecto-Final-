#include "nivel.h"

Nivel::Nivel(QObject* parent) : QObject(parent) {
    escena = new QGraphicsScene();
    fondo = QPixmap();
    terminado = false;
}

void Nivel::iniciarnivel() {

}

void Nivel::actualizar() {

}

void Nivel::terminar() {
    terminado = true;
    escena->clear();
}

void Nivel::mostrarCinematicaFinal() {
}
