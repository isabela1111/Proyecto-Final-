#ifndef NIVEL1_H
#define NIVEL1_H

#include "nivel.h"
#include "taopaipai.h"
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QMediaPlayer>
#include <QAudioOutput>

#include <QTimer>

class Nivel1 : public Nivel {
    Q_OBJECT

public:
    explicit Nivel1(QGraphicsView* vista, QObject* parent = nullptr);

    void iniciarnivel() override;
    void comenzarJuego();
    void quitarVida();

    bool gameOverShown;
    bool juegoIniciado;

    QGraphicsTextItem* textoInicio = nullptr;
    QGraphicsTextItem* textoAltura;
    int metrosSubidos;
    void detenerMusica() override;

private slots:
    void actualizarCronometro();
    void iniciarFisica();

private:
    void verificarColisiones();
    void aplicarGravedad();
    void verificarCaida();
    void crearBarrasVida();
    void mostrarPantallaGameOver();
    void mostrarPantallaVictoria();
    void ajustarDificultad();
    void generarPiedra();

    // Elementos de escena
    QGraphicsPixmapItem* fondoItem = nullptr;
    QList<QGraphicsPixmapItem*> fondosScroll;
    QList<QGraphicsRectItem*> barrasVida;
    QGraphicsTextItem* textoCronometro;

    // Estado y logica
    TaoPaiPai* taoPaiPai;
    QGraphicsView* vista;
    int vidasTotales;
    int vidasRestantes;
    int tiempoRestante;
    bool fisicaActiva = false;
    int contadorScroll;
    int scrollsDesdeUltimaPiedra = 0;
    qint64 tiempoUltimaPiedra;



    QTimer* temporizador;
    QTimer* timerCronometro;
    QTimer* timerPiedras;
    int piedrasPorIntervalo;


    QMediaPlayer* musicaNivel1;
    QAudioOutput* salidaAudio1;


    const int ALTURA_SECCION = 600;
};

#endif // NIVEL1_H
