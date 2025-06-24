#include "personaje.h"
#include <QDebug>
#include <QKeyEvent>

Personaje::Personaje(QGraphicsView* vista, QObject* parent)
    : QObject(parent), QGraphicsPixmapItem()
{
    posX = 0;
    posY = 0;
    velocidad = 0;
    spriteX = 0;
    spriteY = 0;
    spriteAncho = 60;
    spriteAlto = 60;
    limites = QSize(800, 600);
}

void Personaje::recibirDanio(int cantidad) {
    vida -= cantidad;
    qDebug() << nombre << "recibió" << cantidad << "de daño. Vida restante:" << vida;
}

void Personaje::mover() {
    // Se sobreescribe en el hijo
}

void Personaje::saltar() {
    // Se sobreescribe en el hijo
}

void Personaje::atacar() {
    // Se sobreescribe en el hijo
}

void Personaje::esquivar() {
    // Se sobreescribe en el hijo
}


void Personaje::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
    case Qt::Key_Up:
        if (y() > 0) {
            mover();
        }
        break;
    case Qt::Key_Down:
        if (y() + boundingRect().height() < limites.height()) {
            moveBy(0, velocidad);  // Solo bajar (sin animación)
        }
        break;
    case Qt::Key_Space:
        saltar();
        break;
    default:
        break;
    }
}
