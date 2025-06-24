#ifndef NIVEL1_H
#define NIVEL1_H

#include "nivel.h"
#include "taopaipai.h"
#include <QGraphicsView>

class Nivel1 : public Nivel {
    Q_OBJECT
public:
    explicit Nivel1(QGraphicsView* vista, QObject* parent = nullptr);

    void iniciarnivel() override;
    void generarObstaculos();
    void verificarCaida();
    void aplicarGravedad();

private:
    float alturaActual;
    float alturaMeta;
    TaoPaiPai* taoPaiPai;
    QGraphicsView* vista;
};

#endif // NIVEL1_H
