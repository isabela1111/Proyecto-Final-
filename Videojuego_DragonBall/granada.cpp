#include "granada.h"
#include <QPixmap>
#include <QSoundEffect>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QDebug>

Granada::Granada(qreal xInicial, qreal yInicial, QGraphicsItem* objetivoJugador, bool esDevuelta)
    : objetivo(objetivoJugador), devuelta(esDevuelta) {

    hojaGranada.load("Recursos::granadaSprite");
    spriteAncho = 32;
    spriteAlto = 32;
    frameActual = 0;
    frameExplosion = 0;
    explotando = false;

    setPixmap(hojaGranada.copy(0, 0, spriteAncho, spriteAlto));
    setPos(xInicial, yInicial);

    // Direcciones dependiendo si fue devuelta o lanzada por el jefe
    velocidadX = devuelta ? 5.5 : -5.5;
    velocidadY = -8;
    gravedad = 0.4;

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Granada::mover);
    timer->start(50);
}

void Granada::mover() {
    if (explotando) {
        if (frameExplosion < 7) {
            setPixmap(hojaGranada.copy(frameExplosion * spriteAncho, 32, spriteAncho, spriteAlto));
            frameExplosion++;
        } else {
            deleteLater();
        }
        return;
    }

    velocidadY += gravedad;
    moveBy(velocidadX, velocidadY);

    frameActual = (frameActual + 1) % 8;
    setPixmap(hojaGranada.copy(frameActual * spriteAncho, 0, spriteAncho, spriteAlto));

    if (y() > 550) {
        explotar();
        return;
    }

    if (objetivo && collidesWithItem(objetivo)) {
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
    setPixmap(hojaGranada.copy(0, 32, spriteAncho, spriteAlto));
    velocidadX = 0;
    velocidadY = 0;

    QSoundEffect* sonidoExplosion = new QSoundEffect(this);
    sonidoExplosion->setSource(QUrl("qrc:/Recursos/Sonidos/explosion.wav"));
    sonidoExplosion->play();
}

