#include "nivel.h"
#include <QGraphicsView>

Nivel::Nivel(QGraphicsView* vista_, QObject* parent)
    : QObject(parent), vista(vista_)
{
    escena = new QGraphicsScene();
    if (vista) {
        vista->setScene(escena);
    }
}



void Nivel::iniciarnivel() {

}

void Nivel::actualizar() {

}

void Nivel::terminar() {
    terminado = true;
    escena->clear();
}

