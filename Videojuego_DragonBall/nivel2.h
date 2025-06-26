#ifndef NIVEL2_H
#define NIVEL2_H

#include "nivel.h"
#include "goku.h"
#include "taopaipaijefe.h"
#include <QGraphicsRectItem>

class Nivel2 : public Nivel {
    Q_OBJECT
public:
    explicit Nivel2(QObject* parent = nullptr);
    void iniciarnivel() override;
    void actualizar() override;
    void mostrarPantallaGameOver();
    void mostrarPantallaVictoria();
    void crearBarrasVida();

private:
    Goku* goku;
    TaoPaiPaiJefe* taoPaiPai;
    QTimer* timerAtaques;
    QVector<QGraphicsRectItem*> barrasVidaGoku;
    QVector<QGraphicsRectItem*> barrasVidaTao;
    bool victoriaMostrada = false;
    bool derrotaMostrada = false;

    void actualizarBarrasVida();
};

#endif // NIVEL2_H
