#ifndef GOKU_H
#define GOKU_H

#include "personaje.h"

class Goku : public Personaje {
    Q_OBJECT
public:
    explicit Goku(QGraphicsView* vista = nullptr, QObject* parent = nullptr);

    void mover() override;
    void saltar() override;
    void atacar() override;

    int getVida() const { return vida; }
    bool tieneNubeVoladora;
    bool modoAtaque;

    void usarNube();
    void devolverGranada();

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    QPixmap hojaSprites;
    int frameActual;
    int tiempoSalto;
    bool enElAire;
};

#endif // GOKU_H
