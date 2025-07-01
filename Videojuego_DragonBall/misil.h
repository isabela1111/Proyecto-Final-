#ifndef MISIL_H
#define MISIL_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>

class GokuNube;

class Misil : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Misil(GokuNube* goku, QGraphicsItem* parent = nullptr);

signals:
    void colisionaConGoku();

private slots:
    void mover();

private:
    QPixmap hojaSprites;
    QTimer* timerMovimiento;
    int velocidad;
    GokuNube* goku;
};

#endif // MISIL_H
