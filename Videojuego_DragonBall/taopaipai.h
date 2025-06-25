#ifndef TAOPAIPAI_H
#define TAOPAIPAI_H

#include "personaje.h"
#include <QTimer>
#include <QPixmap>

class TaoPaiPai : public Personaje {
    Q_OBJECT
public:
    explicit TaoPaiPai(QGraphicsView* vista, QObject* parent = nullptr);

    bool cayendo = false;


    void mostrarCaida();
    void mover() override;
    void saltar() override;

private slots:
    void animarCaminar();
    void animarSalto();
    void actualizarFisica();

private:
    void actualizarFrame();

    QTimer* timerCaminar;
    QTimer* timerSaltar;
    QTimer* timerFisica;

    QPixmap hojaSprites;
    QPixmap sprite;

    int frameActual;
    int filaMaxima;
    int spriteAncho;
    int spriteAlto;

    float velocidadY;
    float gravedad;

    bool enElAire;
    bool estaMoviendose;


};

#endif // TAOPAIPAI_H
