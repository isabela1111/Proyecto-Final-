#include "goku.h"
#include <QKeyEvent>
#include <QDebug>
#include <QTimer>
#include "recursos.h"

Goku::Goku(QGraphicsView* vista, QObject* parent)
    : Personaje(vista, parent),
      frameActual(0),
      tiempoSalto(0),
      enElAire(false),
      modoAtaque(false)
{
    nombre = "Goku";
    vida = 5;
    velocidad = 7;
    spriteAncho = 60;
    spriteAlto = 60;

    hojaSprites.load(Recursos::gokuSprite);
    setPixmap(hojaSprites.copy(0, 0, spriteAncho, spriteAlto));
    setPos(100, 400);

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    temporizadorAtaque = new QTimer(this);
    temporizadorAtaque->setSingleShot(true);
    connect(temporizadorAtaque, &QTimer::timeout, this, [this]() {
        modoAtaque = false;
    });
}

void Goku::mover() {
    frameActual = (frameActual + 1) % 4;
    setPixmap(hojaSprites.copy(frameActual * spriteAncho, 0, spriteAncho, spriteAlto));
    // movimiento ya se hace en keyPressEvent
}

void Goku::saltar() {
    if (!enElAire) {
        tiempoSalto = 15;
        enElAire = true;
    }
}

void Goku::atacar() {
    qDebug() << "Goku realiza un ataque.";
    setPixmap(hojaSprites.copy(0, 60, spriteAncho, spriteAlto));
    modoAtaque = true;
    temporizadorAtaque->start(300); // ataque activo por 300 ms
}

void Goku::devolverGranada() {
    modoAtaque = true;
    temporizadorAtaque->start(300);
    qDebug() << "Goku está intentando devolver la granada.";
}

bool Goku::estaEnModoAtaque() const {
    return modoAtaque;
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
