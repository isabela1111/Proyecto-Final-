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
    void setVelocidad(float v);
    void setMaxMisiles(int m);

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

    float baseY;
    float amplitud;
    float omega;
    float tiempo;
    float velocidadX;

    float velocidad = 2.0;
    int maxMisiles = 1;

    bool esEstacionario;
    int misilesDisparados;
    bool haTerminadoDisparos = false;
};

#endif // AVIONENEMIGO_H
