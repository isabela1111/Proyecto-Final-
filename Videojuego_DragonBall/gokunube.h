#ifndef GOKUNUBE_H
#define GOKUNUBE_H

#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QTimer>
#include <QKeyEvent>
#include <QObject>

class GokuNube : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    GokuNube(QGraphicsView* vista, QObject* parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void cambiarSprite();

private:
    QGraphicsView* vista;
    QPixmap hojaSprites;
    QTimer* timerAnimacion;
    int frameActual;
    int velocidad;
    void moverAutomatico();

    QTimer* timerMovimiento;

};

#endif // GOKUNUBE_H
