#ifndef GOKUNUBE_H
#define GOKUNUBE_H

#include "personaje.h"

#include <QTimer>
#include <QKeyEvent>
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>

class GokuNube : public Personaje {
    Q_OBJECT
public:
    GokuNube(QGraphicsView* vista, QObject* parent = nullptr);
    ~GokuNube();
    void recibirDanio(int cantidad) override;


protected:
    void keyPressEvent(QKeyEvent* event) override;
signals:
    void gokuRecibeDanio();


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
