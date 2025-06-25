#ifndef NIVEL1_H
#define NIVEL1_H

#include "nivel.h"
#include "taopaipai.h"
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>

class Nivel1 : public Nivel {
    Q_OBJECT
public:
    explicit Nivel1(QGraphicsView* vista, QObject* parent = nullptr);

    void iniciarnivel() override;
    void generarObstaculos();
    void verificarCaida();
    void aplicarGravedad();

private slots:
    void actualizarCronometro();

private:
    float alturaActual;
    float alturaMeta;

    TaoPaiPai* taoPaiPai;
    QGraphicsView* vista;

    QList<QGraphicsRectItem*> barrasVida;
    int vidasTotales;
    int vidasRestantes;

    int tiempoRestante;
    QGraphicsTextItem* textoCronometro;

    bool gameOverShown;
    bool juegoIniciado;


    void crearBarrasVida();
    void actualizarBarraVida();
    void mostrarPantallaGameOver();
    void mostrarPantallaVictoria();
    void comenzarJuego();
};

#endif // NIVEL1_H
