#ifndef TAOPAIPAI_H
#define TAOPAIPAI_H

#include "personaje.h"

#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QSoundEffect>
#include <QKeyEvent>
#include <QGraphicsView>

class TaoPaiPai : public Personaje {
    Q_OBJECT
public:
    explicit TaoPaiPai(QGraphicsView* vista, QObject* parent = nullptr);

    void mover() override;
    void saltar() override;

    void keyPressEvent(QKeyEvent* event) override;
    void mostrarCaida();
    void reproducirSonidoGolpe();
    void reiniciarFisica();
    void activarFisica(bool activo);

    bool cayendo;

private slots:
    void animarCaminar();
    void animarSalto();
    void actualizarFisica();

private:
    // Estado y fisicas
    float posX;
    float posY;
    float velocidadY;
    float gravedad;
    bool enElAire;
    bool estaMoviendose;
    bool puedeSaltar = true;

    // Sprite y animaciones
    QPixmap hojaSprites;
    QPixmap sprite;
    int frameActual;
    int spriteAncho;
    int spriteAlto;
    int filaMaxima;

    // Timers
    QTimer* timerCaminar;
    QTimer* timerSaltar;
    QTimer* timerFisica;

    // Sonidos
    QSoundEffect* efectoSalto;
    QSoundEffect* efectoGolpe;

    void actualizarFrame();
};

#endif // TAOPAIPAI_H
