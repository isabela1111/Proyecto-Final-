#include "taopaipai.h"
#include <QTimer>
#include <QDebug>

TaoPaiPai::TaoPaiPai(QGraphicsView* vista, QObject* parent)
    : Personaje(vista, parent)
{
    nombre = "Tao Pai Pai";
    vida = 5;
    velocidad = 5;
    modoAtaque = false;
    tieneGranada = true;
    subiendoTorre = true;

    posX = 400;
    posY = 300;
    spriteX = 0;
    spriteY = 0;
    spriteAncho = 60;
    spriteAlto = 60;

    hojaSprites.load(":/sprites.png");
    sprite = hojaSprites.copy(spriteX, spriteY, spriteAncho, spriteAlto);
    setPixmap(sprite);
    setPos(posX, posY);

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    QTimer* ataqueTimer = new QTimer(this);
    connect(ataqueTimer, &QTimer::timeout, this, &TaoPaiPai::ataqueAutomatico);
    ataqueTimer->start(3000);
}


void TaoPaiPai::caer() {
    setY(y() + 200);
}

void TaoPaiPai::lanzarGranada() {
    if (tieneGranada) {

        qDebug("Tao Pai Pai lanza una granada.");
        tieneGranada = false;
    }
}


void TaoPaiPai::ataqueAutomatico() {
    if (modoAtaque) {
        lanzarGranada();
    }
}
