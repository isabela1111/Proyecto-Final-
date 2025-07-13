#include "granada.h"
#include <QSoundEffect>
#include <QGraphicsScene>
#include <QDebug>
#include "recursos.h"
#include "personaje.h"

Granada::Granada(qreal xInicial, qreal yInicial, QGraphicsItem* objetivoJugador, bool esDevuelta)
    : Obstaculo(nullptr), objetivo(objetivoJugador), devuelta(esDevuelta),
    explotando(false), yaColisiono(false)
{
    hojaGranada.load(Recursos::granadaSprite);
    explosionSprite.load(Recursos::explosionSprite);
    spriteAncho = hojaGranada.width() / 17;
    spriteAlto = hojaGranada.height();
    frameActual = 0;
    frameExplosion = 0;

    if (!hojaGranada.isNull()) {
        setPixmap(hojaGranada.copy(0, 0, spriteAncho, spriteAlto));
    }
    else {
        qDebug() << "Error: hojaGranada está vacía.";
    }
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
        int explosionAncho = 32;
        int explosionAlto = 45;
        if (frameExplosion < 12) {
            setPixmap(explosionSprite.copy(frameExplosion * explosionAncho, 0, explosionAncho, explosionAlto));
            frameExplosion++;
        }
        else {
            if (scene()) scene()->removeItem(this);
            deleteLater();
        }
        return;
    }
    velocidadY += gravedad;
    moveBy(velocidadX, velocidadY);
    if (!hojaGranada.isNull() && (frameActual * spriteAncho < hojaGranada.width())) {
        setPixmap(hojaGranada.copy(frameActual * spriteAncho, 0, spriteAncho, spriteAlto));
        frameActual = (frameActual + 1) % 17;
    }
    if (!yaColisiono && objetivo && scene() && objetivo->scene() == scene() && collidesWithItem(objetivo)) {
        yaColisiono = true;
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
    frameExplosion = 0;
    velocidadX = 0;
    velocidadY = 0;
    setPixmap(explosionSprite.copy(0, 0, 32, 45));

    QSoundEffect* sonidoExplosion = new QSoundEffect(this);
    sonidoExplosion->setSource(QUrl("qrc:/Recursos/Sonidos/explosion.wav"));
    sonidoExplosion->play();
}
