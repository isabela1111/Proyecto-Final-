#ifndef GOKUNUBE_H
#define GOKUNUBE_H

#include "personaje.h"

#include <QTimer>
#include <QKeyEvent>
#include <QObject>
#include <QGraphicsView>

class GokuNube : public Personaje {
    Q_OBJECT
public:
    GokuNube(QGraphicsView* vista, QObject* parent = nullptr);
    ~GokuNube();

protected:
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void cambiarSprite();

private:
    QTimer* timerAnimacion;
    QTimer* timerMovimiento;
    int frameActual;

    void moverAutomatico();
};

#endif // GOKUNUBE_H
