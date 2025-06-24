#ifndef NIVEL1_H
#define NIVEL1_H
#include "nivel.h"
#include "taopaipai.h"

class Nivel1 : public Nivel {
    Q_OBJECT
public:
    explicit Nivel1(QObject* parent = nullptr);

    float alturaActual;
    float alturaMeta;
    TaoPaiPai* taoPaiPai;

    void generarObstaculos();
    void verificarCaida();
    void aplicarGravedad();
};
#endif // NIVEL1_H
