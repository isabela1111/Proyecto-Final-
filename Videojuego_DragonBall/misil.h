#ifndef MISIL_H
#define MISIL_H

#include "obstaculo.h"

#include <QPixmap>

class GokuNube;

class Misil : public Obstaculo {
    Q_OBJECT
public:
    Misil(GokuNube* goku, QGraphicsItem* parent = nullptr);

private slots:
    void mover();

private:
    void limpiarYEliminar();
    QTimer* timerMovimiento;
    QTimer* timerAnimacion;
    QPixmap sprite;
    GokuNube* goku;
    int frameActual;
};

#endif // MISIL_H


