#ifndef GOKUNUBE_H
#define GOKUNUBE_H

#include <QTimer>
#include <QKeyEvent>
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>

class GokuNube : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    GokuNube(QGraphicsView* vista, QObject* parent = nullptr);
    ~GokuNube();

protected:
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void cambiarSprite();

private:
    QGraphicsView* vista;
    QPixmap hojaSprites;
    QTimer* timerAnimacion;
    QTimer* timerMovimiento;
    int frameActual;
    int velocidad;

    void moverAutomatico();
};

#endif // GOKUNUBE_H
