#include "granada.h"
#include <QPixmap>
#include <QSoundEffect>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QDebug>
#include "recursos.h"

Granada::Granada(qreal xInicial, qreal yInicial, QGraphicsItem* objetivoJugador, bool esDevuelta)
    : objetivo(objetivoJugador), devuelta(esDevuelta) {

    hojaGranada.load(Recursos::granadaSprite);
    spriteAncho = 60;  // CAMBIADO de 32 a 60
    spriteAlto = 60;
    frameActual = 0;
    frameExplosion = 0;
    explotando = false;

    setPixmap(hojaGranada.copy(0, 0, spriteAncho, spriteAlto));
    setPos(xInicial, yInicial);

    velocidadX = devuelta ? 5.5 : -5.5;
    velocidadY = -8;
    gravedad = 0.4;

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Granada::mover);
    timer->start(50);
}

void Granada::mover() {
    if (explotando) {
        if (frameExplosion < 11) {
            setPixmap(hojaGranada.copy(frameExplosion * spriteAncho, spriteAlto, spriteAncho, spriteAlto));
            frameExplosion++;
        } else {
            deleteLater();
        }
        return;
    }

    velocidadY += gravedad;
    moveBy(velocidadX, velocidadY);

    frameActual = (frameActual + 1) % 18;
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
    setPixmap(hojaGranada.copy(0, spriteAlto, spriteAncho, spriteAlto));
    velocidadX = 0;
    velocidadY = 0;

    QSoundEffect* sonidoExplosion = new QSoundEffect(this);
    sonidoExplosion->setSource(QUrl("qrc:/Recursos/Sonidos/explosion.wav"));
    sonidoExplosion->play();
}

