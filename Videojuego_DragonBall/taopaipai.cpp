#include "taopaipai.h"
#include "recursos.h"
#include <QDebug>

TaoPaiPai::TaoPaiPai(QGraphicsView* vista, QObject* parent)
    : Personaje(vista, parent), frameActual(0), filaMaxima(5)  // 5 frames verticales
{
    nombre = "Tao Pai Pai";
    vida = 5;
    velocidad = 5;

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

    timerCaminar = new QTimer(this);
    connect(timerCaminar, &QTimer::timeout, this, &TaoPaiPai::animarCaminar);

    timerSaltar = new QTimer(this);
    connect(timerSaltar, &QTimer::timeout, this, &TaoPaiPai::animarSalto);
}

void TaoPaiPai::actualizarFrame() {
    int yFrame = frameActual * spriteAlto;
    sprite = hojaSprites.copy(0, yFrame, spriteAncho, spriteAlto);
    setPixmap(sprite);
}

void TaoPaiPai::mover() {
    frameActual = 0;
    hojaSprites.load(Recursos::TaoRunSprite);
    hojaSprites.setMask(hojaSprites.createMaskFromColor(QColor(128, 0, 128), Qt::MaskInColor));
    timerCaminar->start(100);
}

void TaoPaiPai::animarCaminar() {
    if (frameActual >= filaMaxima) {
        frameActual = 0;
        timerCaminar->stop();
        return;
    }

    actualizarFrame();
    moveBy(0, -velocidad);  // Subir

    frameActual++;
}

void TaoPaiPai::saltar() {
    frameActual = 0;
    hojaSprites.load(Recursos::TaoJumpSprite);
    hojaSprites.setMask(hojaSprites.createMaskFromColor(QColor(128, 0, 128), Qt::MaskInColor));
    timerSaltar->start(100);
}

void TaoPaiPai::animarSalto() {
    if (frameActual >= filaMaxima) {
        frameActual = 0;
        timerSaltar->stop();
        mover();  // Al finalizar salto, retoma la animación de subir
        return;
    }

    actualizarFrame();
    moveBy(0, -10);  // Subida más fuerte

    frameActual++;
}

