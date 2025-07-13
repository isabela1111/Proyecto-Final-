#include "personaje.h"
#include "nivel1.h"

#include <QDebug>
#include <QKeyEvent>

Personaje::Personaje(QGraphicsView* vista, QObject* parent)
    : QObject(parent), QGraphicsPixmapItem(), vista(vista)
{
    posX = 0;
    posY = 0;
    velocidad = 5;
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
}

void Personaje::saltar() {
}

void Personaje::atacar() {
}

void Personaje::esquivar() {
}

int Personaje::getVida() const {
    return vida;
}

void Personaje::keyPressEvent(QKeyEvent* event) {
    Nivel1* nivel = qobject_cast<Nivel1*>(parent());
    if (nivel && !nivel->juegoIniciado && !nivel->gameOverShown) {
        if (event->key() == Qt::Key_E) {
            nivel->comenzarJuego();
            if (nivel->textoInicio) {
                nivel->escena->removeItem(nivel->textoInicio);
                delete nivel->textoInicio;
                nivel->textoInicio = nullptr;
            }
        }
        return;
    }
    switch (event->key()) {
    case Qt::Key_Up:
        if (y() > 0) mover();
        break;
    case Qt::Key_Down:
        if (y() + boundingRect().height() < limites.height()) {
            moveBy(0, velocidad);
        }
        break;
    case Qt::Key_Space:
        saltar();
        break;
    default:
        break;
    }
}


