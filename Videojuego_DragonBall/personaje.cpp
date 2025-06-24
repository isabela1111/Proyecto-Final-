#include "personaje.h"
#include <QGraphicsView>

Personaje::Personaje(QGraphicsView* vista, QObject* parent)
    : QObject(parent), posX(200), posY(200), velocidad(5) {
    spriteX = 0;
    spriteY = 0;
    spriteAncho = 60;
    spriteAlto = 60;
    vida = 5;

    if (vista)
        limites = vista->size();

    setFlag(QGraphicsItem::ItemIsFocusable);
    hojaSprites.load(":/sprites.png");
    sprite = hojaSprites.copy(spriteX, spriteY, spriteAncho, spriteAlto);
    setPixmap(sprite);
    setPos(posX, posY);
    setFocus();
}

void Personaje::recibirDanio(int cantidad) {
    vida -= cantidad;
}

void Personaje::mover() {
    // logica de movimiento
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
    case Qt::Key_A: // izquierda
        if (posX - velocidad >= 0)
            posX -= velocidad;
        break;
    case Qt::Key_D: // derecha
        if (posX + spriteAncho + velocidad <= limites.width())
            posX += velocidad;
        break;
    case Qt::Key_W: // arriba
        if (posY - velocidad >= 0)
            posY -= velocidad;
        break;
    case Qt::Key_S: // abajo
        if (posY + spriteAlto + velocidad <= limites.height())
            posY += velocidad;
        break;
    }
    setPos(posX, posY);
}
