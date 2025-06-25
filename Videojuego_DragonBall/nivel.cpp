#include "nivel.h"
#include <QGraphicsView>

Nivel::Nivel(QObject* parent)
    : QObject(parent)
{
    escena = new QGraphicsScene();
    vista = new QGraphicsView(escena);
    vista->setFixedSize(800, 600);
    vista->show();
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
