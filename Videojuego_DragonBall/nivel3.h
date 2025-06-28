#ifndef NIVEL3_H
#define NIVEL3_H

#include "nivel.h"
#include "gokunube.h"
#include "avionenemigo.h"

#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QVector>

class Nivel3 : public Nivel
{
    Q_OBJECT
public:
    Nivel3(QGraphicsView* vista, QObject* parent = nullptr);
    void iniciarnivel() override;
    void actualizar() override;

private:
    QGraphicsView* vista;
    GokuNube* goku;
    int distanciaRecorrida;
    int vidas;
    QGraphicsTextItem* textoDistancia;
    QVector<QGraphicsRectItem*> barrasVida;

    QTimer* timerObstaculos;
    QTimer* timerDistancia;

    void generarObstaculo();
    void actualizarDistancia();
    void perderVida();
    void mostrarGameOver();
};

#endif // NIVEL3_H
