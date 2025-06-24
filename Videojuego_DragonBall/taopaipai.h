#ifndef TAOPAIPAI_H
#define TAOPAIPAI_H

#include "personaje.h"
#include <QTimer>
#include <QPixmap>

class TaoPaiPai : public Personaje {
    Q_OBJECT
public:
    explicit TaoPaiPai(QGraphicsView* vista, QObject* parent = nullptr);

    void mover() override;
    void saltar() override;

private slots:
    void animarCaminar();
    void animarSalto();

private:
    void actualizarFrame();

    QTimer* timerCaminar;
    QTimer* timerSaltar;

    QPixmap hojaSprites;
    QPixmap sprite;

    int frameActual;
    int filaMaxima;
    int spriteAncho;
    int spriteAlto;
};

#endif // TAOPAIPAI_H
