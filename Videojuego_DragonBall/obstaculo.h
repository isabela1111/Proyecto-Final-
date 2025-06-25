#ifndef OBSTACULO_H
#define OBSTACULO_H

#include <QGraphicsPixmapItem>
#include <QTimer>

class Nivel1;  // Para poder acceder a nivel

class Obstaculo : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Obstaculo(Nivel1* nivel, QGraphicsItem* parent = nullptr);
    virtual void mover();  // Movimiento base
    virtual void verificarColision();  // Para sobreescribir si quieres otros efectos

protected:
    Nivel1* nivel;
    QTimer* timerMovimiento;
};

#endif // OBSTACULO_H
