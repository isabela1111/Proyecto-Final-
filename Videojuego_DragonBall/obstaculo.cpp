#include "obstaculo.h"

Obstaculo::Obstaculo(QPixmap sprite, int x, int y) {
    setPixmap(sprite);
    setPos(x, y);
}
