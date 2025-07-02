#ifndef GOKUNUBE_H
#define GOKUNUBE_H

#include <QTimer>
#include <QKeyEvent>
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include "personaje.h"

class GokuNube : public Personaje {
    Q_OBJECT
public:
    GokuNube(QGraphicsView* vista, QObject* parent = nullptr);
    ~GokuNube();

protected:
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void cambiarSprite();
    void moverAutomatico();

private:
    QPixmap hojaSprites;
    QTimer* timerAnimacion;
    QTimer* timerMovimiento;
    int frameActual;
};

#endif // GOKUNUBE_H
