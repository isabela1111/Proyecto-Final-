#include "goku.h"
#include <QTimer>
#include <QDebug>

Goku::Goku(QObject* parent) : Personaje(parent) {
    spriteX = 0;
    spriteY = 0;
    spriteAncho = 60;
    spriteAlto = 60;
    vida = 3;
    tieneNubeVoladora = false;
    modoAtaque = false;

    hojaSprites.load(":/sprites.png");
    sprite = hojaSprites.copy(spriteX, spriteY, spriteAncho, spriteAlto);
    setPixmap(sprite);
    setPos(100, 300);  // Posición inicial
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
}

void Goku::usarNube() {
    tieneNubeVoladora = true;
    qDebug() << "Goku ha activado la nube voladora.";

}


void Goku::devolverGranada() {
    modoAtaque = true;
    qDebug() << "Goku está intentando devolver la granada.";


}
