#ifndef OBSTACULO_H
#define OBSTACULO_H

#include <QGraphicsPixmapItem>
#include <QTimer>

class Nivel1;

class Obstaculo : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Obstaculo(Nivel1* nivel, QGraphicsItem* parent = nullptr);
    virtual void mover();
    virtual void verificarColision();

protected:
    Nivel1* nivel;
    QTimer* timerMovimiento;
};

#endif // OBSTACULO_H
