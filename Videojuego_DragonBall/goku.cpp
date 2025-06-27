#include "goku.h"
#include <QKeyEvent>
#include <QDebug>

Goku::Goku(QGraphicsView* vista, QObject* parent)
    : Personaje(vista, parent),
      frameActual(0),
      tiempoSalto(0),
      enElAire(false)
{
    nombre = "Goku";
    vida = 5;
    velocidad = 7;
    spriteAncho = 60;
    spriteAlto = 60;

    hojaSprites.load("Recusos::gokuSprite");
    setPixmap(hojaSprites.copy(0, 0, spriteAncho, spriteAlto));
    setPos(100, 400);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
}

void Goku::mover() {
    frameActual = (frameActual + 1) % 4;
    setPixmap(hojaSprites.copy(frameActual * spriteAncho, 0, spriteAncho, spriteAlto));
    moveBy(velocidad, 0);
}

void Goku::saltar() {
    if (!enElAire) {
        tiempoSalto = 15;
        enElAire = true;
    }
}

void Goku::atacar() {
    qDebug() << "Goku realiza un ataque básico.";
    setPixmap(hojaSprites.copy(0, 60, spriteAncho, spriteAlto));
}

void Goku::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
    case Qt::Key_A: // izquierda
        if (x() > 0) moveBy(-velocidad, 0);
        mover();
        break;
    case Qt::Key_D: // derecha
        if (x() + boundingRect().width() < limites.width()) moveBy(velocidad, 0);
        mover();
        break;
    case Qt::Key_Space:
        atacar();
        break;
    case Qt::Key_W:
        saltar();
        break;
    default:
        break;
    }

    // Simulación simple de salto
    if (tiempoSalto > 0) {
        moveBy(0, -10);
        tiempoSalto--;
    } else if (enElAire) {
        moveBy(0, 10); // gravedad
        if (y() >= 400) {
            enElAire = false;
            setY(400);
        }
    }
}

void Goku::usarNube() {
    tieneNubeVoladora = true;
    qDebug() << "Goku ha activado la nube voladora.";
}

void Goku::devolverGranada() {
    modoAtaque = true;
    qDebug() << "Goku está intentando devolver la granada.";
}
