#ifndef AVIONENEMIGO_H
#define AVIONENEMIGO_H

#include "obstaculo.h"
#include <QObject>
#include <QTimer>

class GokuNube;
class Misil;

class AvionEnemigo : public Obstaculo {
    Q_OBJECT
public:
    AvionEnemigo(GokuNube* goku, QGraphicsItem* parent = nullptr);

signals:
    void colisionaConGoku();
    void disparoMisil(Misil* misil);

private slots:
    void mover();
    void animar();
    void disparar();

private:
    QTimer* timerMovimiento;
    QTimer* timerAnimacion;
    QTimer* timerDisparo;

    QPixmap hojaSprites;
    int frameActual;
    int totalFrames;
    int spriteAncho;
    int spriteAlto;
    GokuNube* goku;

    bool esEstacionario;
    int misilesDisparados;
    const int maxMisiles = 2;
    bool haTerminadoDisparos = false;
};

#endif // AVIONENEMIGO_H
