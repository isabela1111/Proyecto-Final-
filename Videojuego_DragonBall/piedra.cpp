#include "piedra.h"
#include "recursos.h"

#include <QGraphicsScene>
#include <stdexcept>
#include <QTimer>
#include <QDebug>

Piedra::Piedra(Nivel1* nivel, QGraphicsItem* parent)
    : Obstaculo(nivel, parent),
    frameActual(0), totalFrames(8), anchoFrame(81), altoFrame(79)
{
    if (!hojaSprites.load(Recursos::RocaSprite))
        throw std::runtime_error("Error: no se pudo cargar el sprite de la roca.");
    setZValue(1);
    setPos(285, 2);
    actualizarFrame();
    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, [=]() {
        frameActual = (frameActual + 1) % totalFrames;
        try {
            actualizarFrame();
        } catch (const std::exception& e) {
            qDebug() << "Excepcion en animacion de piedra: " << e.what();
        }
    });
    timerAnimacion->start(100);

    // Timer para movimiento de caída
    timerMovimiento = new QTimer(this);
    connect(timerMovimiento, &QTimer::timeout, this, &Piedra::mover);
    timerMovimiento->start(30);  // cada 30 ms baja
}

void Piedra::actualizarFrame() {
    int fila = 1;
    int x = frameActual * anchoFrame;
    int y = fila * altoFrame;
    if (x + anchoFrame > hojaSprites.width() || y + altoFrame > hojaSprites.height())
        throw std::out_of_range("Error: coordenadas del frame fuera de los límites del sprite.");
    QPixmap frame = hojaSprites.copy(x, y, anchoFrame, altoFrame);
    setPixmap(frame.scaled(30, 30));
}

void Piedra::mover() {
    moveBy(0, 3);  // velocidad de caída
    if (y() > 700) {
        deleteLater();
    }
}
