#ifndef GRANADA_H
#define GRANADA_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QObject>

class Granada : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Granada(qreal xInicial, qreal yInicial, QGraphicsItem* objetivoJugador = nullptr, bool esDevuelta = false);

    void mover();
    void explotar();

private:
    QPixmap hojaGranada;
    QGraphicsItem* objetivo;
    int spriteAncho;
    int spriteAlto;
    int frameActual;
    int frameExplosion;
    bool explotando;
    bool devuelta;

    qreal velocidadX;
    qreal velocidadY;
    qreal gravedad;
};

#endif // GRANADA_H
