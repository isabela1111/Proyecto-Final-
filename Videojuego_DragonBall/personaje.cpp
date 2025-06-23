#include "personaje.h"
#include <QKeyEvent>

Personaje::Personaje(QObject* parent) : QObject(parent) {
    spriteX = 0;
    spriteY = 0;
    spriteAncho = 60;
    spriteAlto = 60;
    vida = 3;

    setFlag(QGraphicsItem::ItemIsFocusable);
    hojaSprites.load(":/sprites.png");
    sprite = hojaSprites.copy(spriteX, spriteY, spriteAncho, spriteAlto);
    setPixmap(sprite);
    setPos(200, 200);
}

void Personaje::recibirDanio(int cantidad) {
    vida -= cantidad;
}

void Personaje::mover() {
    // logica base de movimiento
}

void Personaje::saltar() {
    // por implementar
}

void Personaje::atacar() {
    // por implementar
}

void Personaje::esquivar() {
    // por implementar
}

void Personaje::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
    case Qt::Key_A:
        setX(x() - 5);
        break;
    case Qt::Key_D:
        setX(x() + 5);
        break;
    case Qt::Key_W:
        setY(y() - 5);
        break;
    case Qt::Key_S:
        setY(y() + 5);
        break;
    }
}
