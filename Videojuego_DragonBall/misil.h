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

private slots:
    void mover();

private:
    QTimer* timerMovimiento;
    QPixmap sprite;
    GokuNube* goku;
};

#endif // MISIL_H

