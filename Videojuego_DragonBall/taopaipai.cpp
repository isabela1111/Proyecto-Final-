#include "taopaipai.h"
#include "recursos.h"
#include <QDebug>

TaoPaiPai::TaoPaiPai(QGraphicsView* vista, QObject* parent)
    : Personaje(vista, parent), frameActual(0), filaMaxima(5),
    velocidadY(0), gravedad(0.7), enElAire(false), estaMoviendose(false)
{
    nombre = "Tao Pai Pai";
    vida = 5;
    velocidad = 5;
    cayendo = false;

    spriteAncho = 68;
    spriteAlto = 59;
    posX = 250;
    posY = 500;

    hojaSprites.load(Recursos::TaoRunSprite);
    hojaSprites.setMask(hojaSprites.createMaskFromColor(QColor(128, 0, 128), Qt::MaskInColor));

    actualizarFrame();
    setPos(posX, posY);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    // Animaciones
    timerCaminar = new QTimer(this);
    connect(timerCaminar, &QTimer::timeout, this, &TaoPaiPai::animarCaminar);

    timerSaltar = new QTimer(this);
    connect(timerSaltar, &QTimer::timeout, this, &TaoPaiPai::animarSalto);

    timerFisica = new QTimer(this);
    connect(timerFisica, &QTimer::timeout, this, &TaoPaiPai::actualizarFisica);
    timerFisica->start(30);  // 30 ms para física
}

void TaoPaiPai::actualizarFrame() {
    int yFrame = frameActual * spriteAlto;
    sprite = hojaSprites.copy(0, yFrame, spriteAncho, spriteAlto);
    setPixmap(sprite);
}

void TaoPaiPai::mover() {
    estaMoviendose = true;
    velocidadY = -5;  // Subir
    hojaSprites.load(Recursos::TaoRunSprite);
    frameActual = 0;
    timerCaminar->start(100);
}

void TaoPaiPai::saltar() {
    estaMoviendose = true;
    velocidadY = -12;
    hojaSprites.load(Recursos::TaoJumpSprite);
    frameActual = 0;
    timerSaltar->start(100);
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
        mover();
        return;
    }
    actualizarFrame();
    frameActual++;
}

void TaoPaiPai::actualizarFisica() {
    if (!estaMoviendose) {
        velocidadY += gravedad;}
    moveBy(0, velocidadY);  // SOLO movimiento vertical
    if (y() + boundingRect().height() > 600) {
        mostrarCaida();}
    // Si llega muy arriba, reinicia abajo
    if (y() < 50) {
        setY(500); }
    setX(250);
}

void TaoPaiPai::mostrarCaida() {
    hojaSprites.load(Recursos::TaoCaidoSprite);
    sprite = hojaSprites.copy(0, 0, spriteAncho, spriteAlto);
    setPixmap(sprite);
    velocidadY = 0;
    estaMoviendose = false;
}
