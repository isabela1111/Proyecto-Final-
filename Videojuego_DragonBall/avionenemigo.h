#ifndef AVIONENEMIGO_H
#define AVIONENEMIGO_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>

class GokuNube;

class AvionEnemigo : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    AvionEnemigo(GokuNube* goku, QGraphicsItem* parent = nullptr);

signals:
    void colisionaConGoku();

private slots:
    void mover();
    void animar();

private:
    QTimer* timerMovimiento;
    QTimer* timerAnimacion;
    QPixmap hojaSprites;
    int frameActual;
    int totalFrames;
    int spriteAncho;
    int spriteAlto;
    GokuNube* goku;
};

#endif // AVIONENEMIGO_H
