#ifndef TAOPAIPAIJEFE_H
#define TAOPAIPAIJEFE_H

#include "personaje.h"
#include <QTimer>
#include <QVector>
#include <QPointF>
#include <QGraphicsView>
#include <QPixmap>

class TaoPaiPaiJefe : public Personaje {
    Q_OBJECT

public:
    TaoPaiPaiJefe(QGraphicsView* vista = nullptr, QObject* parent = nullptr);

    void atacar() override;
    void lanzarGranada();
    void setObjetivo(Personaje* objetivo);
    void recibirDanio(int cantidad) override;

private slots:
    void aparecer();
    void desaparecer();

private:
    Personaje* objetivoJugador;

    // Estados y control
    bool estaVisible;
    bool estaCayendo;
    int direccion;

    // Sprites y frames
    QPixmap hojaMovimiento;
    int spriteAncho;
    int spriteAlto;
    int frameAtaque;
    int frameMovimiento;

    // Posiciones posibles
    QVector<QPointF> posicionesAparicion;

    // Temporizadores
    QTimer* aparicionTimer;
    QTimer* desaparicionTimer;
    QTimer* ataqueTimer;
};

#endif // TAOPAIPAIJEFE_H
