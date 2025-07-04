#ifndef PIEDRA_H
#define PIEDRA_H

#include "obstaculo.h"

#include <QTimer>
#include <QPixmap>

class Piedra : public Obstaculo{
public:
    Piedra(Nivel1* nivel, QGraphicsItem* parent = nullptr);

private:
    QPixmap hojaSprites;
    int frameActual;
    int totalFrames;
    int anchoFrame;
    int altoFrame;
    QTimer* timerAnimacion;

    void actualizarFrame();
};

#endif // PIEDRA_H
