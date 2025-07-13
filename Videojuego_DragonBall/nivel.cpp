#include "nivel.h"

#include <QGraphicsView>
#include <stdexcept>

Nivel::Nivel(QGraphicsView* vista_, QObject* parent)
    : QObject(parent), vista(vista_)
{
    escena = new QGraphicsScene();
    if (!escena)
        throw std::runtime_error("Error: no se pudo crear la escena del nivel.");

    if (!vista)
        throw std::invalid_argument("Error: vista no puede ser nula en el nivel.");

    vista->setScene(escena);
}

void Nivel::iniciarnivel() {
}

void Nivel::actualizar() {
}

void Nivel::terminar() {
    if (!escena)
        throw std::runtime_error("Error: escena no inicializada al intentar terminar el nivel.");
    terminado = true;
    escena->clear();
}

void Nivel::detenerMusica() {
}
