#ifndef TAOPAIPAI_H
#define TAOPAIPAI_H

#include "personaje.h"

class TaoPaiPai : public Personaje {
    Q_OBJECT
public:
    explicit TaoPaiPai(QGraphicsView* vista = nullptr, QObject* parent = nullptr);  // ✅ Igual que en Goku

    bool tieneGranada;
    bool subiendoTorre;
    bool modoAtaque;

    void subirTorre();
    void caer();
    void lanzarGranada();
    void ataqueAutomatico();
};

#endif // TAOPAIPAI_H
