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

    bool gameOverShown;
    bool juegoIniciado;
    void iniciarnivel() override;
    void generarObstaculos();
    void verificarCaida();
    void aplicarGravedad();
    void quitarVida();
    void comenzarJuego();

private slots:
    void actualizarCronometro();

private:
    float alturaActual;
    float alturaMeta;
    int vidasTotales;
    int vidasRestantes;
    int tiempoRestante;

    TaoPaiPai* taoPaiPai;
    QGraphicsView* vista;
    QList<QGraphicsRectItem*> barrasVida;
    QGraphicsTextItem* textoCronometro;

    QTimer* temporizador;
    QTimer* timerCronometro;
    QTimer* timerPiedras;

    void crearBarrasVida();
    void actualizarBarraVida();
    void mostrarPantallaGameOver();
    void mostrarPantallaVictoria();
};

#endif // NIVEL1_H
