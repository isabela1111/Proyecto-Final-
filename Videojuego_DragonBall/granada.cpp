#include "granada.h"
#include <QGraphicsScene>
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include <QDebug>
#include "personaje.h"

Granada::Granada(qreal xInicial, qreal yInicial, QGraphicsItem* objetivoJugador, bool esDevuelta)
    : Obstaculo(nullptr), objetivo(objetivoJugador), devuelta(esDevuelta),
    explotando(false), yaColisiono(false) {

    spriteAncho = 32;
    spriteAlto = 32;
    frameActual = 0;

    // Crear círculo rojo temporal como sprite de la granada
    QPixmap pixmap(spriteAncho, spriteAlto);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setBrush(Qt::red);
    painter.setPen(Qt::black);
    painter.drawEllipse(0, 0, spriteAncho, spriteAlto);
    painter.end();

    setPixmap(pixmap);
    setPos(xInicial, yInicial);

    velocidadX = devuelta ? 5.5 : -5.5;
    velocidadY = -8;
    gravedad = 0.4;

    timerMovimiento = new QTimer(this);
    connect(timerMovimiento, &QTimer::timeout, this, &Granada::mover);
    timerMovimiento->start(50);
}

void Granada::mover() {
    if (explotando) {
        return;
    }

    velocidadY += gravedad;
    moveBy(velocidadX, velocidadY);

    if (!yaColisiono && objetivo && collidesWithItem(objetivo)) {
        yaColisiono = true;

        // Aplica daño si NO fue devuelta
        if (!devuelta) {
            Personaje* personajeObjetivo = dynamic_cast<Personaje*>(objetivo);
            if (personajeObjetivo) {
                personajeObjetivo->recibirDanio(1);
                qDebug() << "La granada dañó al jugador.";
            } else {
                qDebug() << "El objetivo no es un Personaje válido.";
            }
        }

        explotar();
        return;
    }

    if (y() > 550) {
        explotar();
    }
}


void Granada::explotar() {
    explotando = true;
    velocidadX = 0;
    velocidadY = 0;

    // Círculo amarillo de explosión
    QPixmap explosion(spriteAncho, spriteAlto);
    explosion.fill(Qt::transparent);

    QPainter painter(&explosion);
    painter.setBrush(Qt::yellow);
    painter.setPen(Qt::black);
    painter.drawEllipse(0, 0, spriteAncho, spriteAlto);
    painter.end();

    setPixmap(explosion);

    QTimer::singleShot(300, this, [this]() {
        if (scene()) scene()->removeItem(this);
        deleteLater();
    });
}
