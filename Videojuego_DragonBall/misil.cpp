#include "misil.h"
#include "recursos.h"
#include "gokunube.h"

#include <QGraphicsScene>
#include <stdexcept>

Misil::Misil(GokuNube* goku, QGraphicsItem* parent)
    : Obstaculo(nullptr, parent), goku(goku), frameActual(0)
{
    if (!sprite.load(Recursos::misilSprite))
        throw std::runtime_error("Error: No se pudo cargar el sprite del misil.");
    if (!goku)
        throw std::invalid_argument("Error: GokuNube es nulo al crear el misil.");
    setPixmap(sprite.copy(0, 0, 34, 20));
    setZValue(2);
    setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
    timerMovimiento = new QTimer(this);
    if (!connect(timerMovimiento, &QTimer::timeout, this, &Misil::mover))
        throw std::runtime_error("Error: No se pudo conectar el timerMovimiento del misil.");
    timerMovimiento->start(30);

    timerAnimacion = new QTimer(this);
    if (!connect(timerAnimacion, &QTimer::timeout, this, [=]() {
            try {
                frameActual = (frameActual + 1) % 3;
                setPixmap(sprite.copy(frameActual * 34, 0, 34, 20));
            }
            catch (const std::exception& e) {
                qDebug() << "Excepción en animación del misil: " << e.what();
            }
        }))
        throw std::runtime_error("Error: No se pudo conectar el timerAnimacion del misil.");
    timerAnimacion->start(150);
}


void Misil::mover() {
    try {
        setX(x() - 4);
        if (!scene() || !goku || !goku->scene()) {
            limpiarYEliminar();
            return;
        }
        QList<QGraphicsItem*> colisiones = collidingItems();
        for (QGraphicsItem* item : colisiones) {
            GokuNube* gokuColisionado = dynamic_cast<GokuNube*>(item);
            if (gokuColisionado) {
                gokuColisionado->recibirDanio(1);
                if (scene()) scene()->removeItem(this);
                limpiarYEliminar();
                return;
            }
        }
        if (x() + boundingRect().width() < 0) {
            if (scene()) scene()->removeItem(this);
            limpiarYEliminar();
        }
    }
    catch (const std::exception& e) {
        qDebug() << "Excepción en Misil::mover:" << e.what();
        limpiarYEliminar();
    }
}


void Misil::limpiarYEliminar() {
    if (timerMovimiento) {
        timerMovimiento->stop();
        timerMovimiento->deleteLater();
        timerMovimiento = nullptr;
    }
    if (timerAnimacion) {
        timerAnimacion->stop();
        timerAnimacion->deleteLater();
        timerAnimacion = nullptr;
    }
    deleteLater();
}

