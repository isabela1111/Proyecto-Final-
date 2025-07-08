#include "taopaipai.h"
#include "recursos.h"
#include <QKeyEvent>
#include <QDebug>

TaoPaiPai::TaoPaiPai(QGraphicsView* vista, QObject* parent)
    : Personaje(vista, parent),velocidadY(0),gravedad(0.7),enElAire(false), estaMoviendose(false),
    frameActual(0), filaMaxima(5)
{
    nombre = "Tao Pai Pai";
    vida = 5;
    velocidad = 5;
    cayendo = false;
    saltando = false;
    puedeSaltar = false;
    saltoDisponible = true;

    spriteAncho = 68;
    spriteAlto = 59;
    posX = 252;
    posY = 500;

    hojaSprites.load(Recursos::TaoRunSprite);
    actualizarFrame();
    setPos(posX, posY);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    timerCaminar = new QTimer(this);
    connect(timerCaminar, &QTimer::timeout, this, &TaoPaiPai::animarCaminar);

    timerSaltar = new QTimer(this);
    connect(timerSaltar, &QTimer::timeout, this, &TaoPaiPai::animarSalto);

    timerFisica = new QTimer(this);
    connect(timerFisica, &QTimer::timeout, this, &TaoPaiPai::actualizarFisica);

    efectoSalto = new QSoundEffect(this);
    efectoSalto->setSource(QUrl("qrc" + Recursos::sonidoSalto));
    efectoSalto->setVolume(0.5);

    efectoGolpe = new QSoundEffect(this);
    efectoGolpe->setSource(QUrl("qrc" + Recursos::sonidoGolpe));
    efectoGolpe->setVolume(0.6);
}

void TaoPaiPai::activarFisica(bool activo) {
    if (activo) {
        if (!timerFisica->isActive())
            timerFisica->start(30);
    } else {
        timerFisica->stop();
    }
}

void TaoPaiPai::actualizarFrame() {
    int yFrame = (filaMaxima - 1 - frameActual) * spriteAlto;
    sprite = hojaSprites.copy(0, yFrame, spriteAncho, spriteAlto);
    setPixmap(sprite);
}

void TaoPaiPai::mover() {
    estaMoviendose = true;
    velocidadY = -5;
    hojaSprites.load(Recursos::TaoRunSprite);
    frameActual = 0;
    timerCaminar->start(100);
    puedeSaltar = true;
}


void TaoPaiPai::saltar() {
    if (!puedeSaltar || !saltoDisponible) return;

    velocidadY = -12;
    hojaSprites.load(Recursos::TaoJumpSprite);
    frameActual = 0;
    timerSaltar->start(100);
    efectoSalto->play();
    saltoDisponible = false;
    puedeSaltar = false;
}


void TaoPaiPai::animarCaminar() {
    if (frameActual >= filaMaxima) {
        frameActual = 0;
        timerCaminar->stop();
        estaMoviendose = false;
        return;
    }
    actualizarFrame();
    frameActual++;
}

void TaoPaiPai::animarSalto() {
    if (frameActual >= filaMaxima) {
        frameActual = 0;
        timerSaltar->stop();
        estaMoviendose = false;
        saltando = false;
        return;
    }
    actualizarFrame();
    frameActual++;
}

void TaoPaiPai::actualizarFisica() {
    if (!estaMoviendose) {
        velocidadY += gravedad;
    }
    moveBy(0, velocidadY);
    if (velocidadY > 0 && !puedeSaltar) {
        puedeSaltar = true;
    }
    if (y() < 50) {
        setY(50);
    }
    setX(250);
}


void TaoPaiPai::mostrarCaida() {
    hojaSprites.load(Recursos::TaoCaidoSprite);
    hojaSprites.setMask(hojaSprites.createMaskFromColor(QColor(128, 0, 128), Qt::MaskInColor));
    sprite = hojaSprites.copy(0, 0, spriteAncho, spriteAlto);
    setPixmap(sprite);
    velocidadY = 0;
    estaMoviendose = false;
    efectoGolpe->play();
}

void TaoPaiPai::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Up || event->key() == Qt::Key_W) {
        mover();
    }
    else if (event->key() == Qt::Key_Space) {
        saltar();
    }
    Personaje::keyPressEvent(event);
}

void TaoPaiPai::keyReleaseEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Space) {
        saltoDisponible = true;
    }
    Personaje::keyReleaseEvent(event);
}


void TaoPaiPai::reproducirSonidoGolpe() {
    if (efectoGolpe) efectoGolpe->play();
}

void TaoPaiPai::reiniciarFisica() {
    velocidadY = 0;
    cayendo = false;
    puedeSaltar = true;
}

