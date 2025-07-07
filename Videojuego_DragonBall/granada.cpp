#include "granada.h"
#include <QSoundEffect>
#include <QGraphicsScene>
#include <QDebug>
#include "recursos.h"

Granada::Granada(qreal xInicial, qreal yInicial, QGraphicsItem* objetivoJugador, bool esDevuelta)
    : Obstaculo(nullptr), objetivo(objetivoJugador), devuelta(esDevuelta),
    explotando(false), yaColisiono(false) {

    hojaGranada.load(Recursos::granadaSprite);
    explosionSprite.load(Recursos::explosionSprite);

    spriteAncho = hojaGranada.width() / 17;
    spriteAlto = hojaGranada.height();  // más seguro

    frameActual = 0;
    frameExplosion = 0;

    if (!hojaGranada.isNull()) {
        setPixmap(hojaGranada.copy(0, 0, spriteAncho, spriteAlto));
    } else {
        qDebug() << "Error: hojaGranada está vacía.";
    }

    setPos(xInicial, yInicial);

    // Movimiento parabólico
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
        } else {
            if (scene()) scene()->removeItem(this);
            deleteLater();
        }
        return;
    }

    // Movimiento parabólico
    velocidadY += gravedad;
    moveBy(velocidadX, velocidadY);

    // Animación de vuelo
    if (!hojaGranada.isNull() && (frameActual * spriteAncho < hojaGranada.width())) {
        setPixmap(hojaGranada.copy(frameActual * spriteAncho, 0, spriteAncho, spriteAlto));
        frameActual = (frameActual + 1) % 17;
    }

    // Limite inferior
    if (y() > 550) {
        explotar();
        return;
    }

    // Colisión con el objetivo
    if (!yaColisiono && objetivo && collidesWithItem(objetivo)) {
        yaColisiono = true;
        explotar();
        if (!devuelta) {
            if (QObject* obj = dynamic_cast<QObject*>(objetivo)) {
                QMetaObject::invokeMethod(obj, "recibirDanio", Q_ARG(int, 1));
            }
        }
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
