#ifndef GRANADA_H
#define GRANADA_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>
#include <QGraphicsScene>

class Granada : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Granada(qreal xInicial, qreal yInicial, QGraphicsItem* objetivo);

public slots:
    void mover();
    void explotar();

private:
    qreal velocidadX;
    qreal velocidadY;
    qreal gravedad;

    QPixmap hojaGranada;
    int frameActual;
    int spriteAncho;
    int spriteAlto;

    bool explotando;
    int frameExplosion;

    QGraphicsItem* objetivo;
};

#endif //GRANADA_H
