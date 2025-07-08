#ifndef NIVEL3_H
#define NIVEL3_H

#include "nivel.h"

#include "gokunube.h"
#include "avionenemigo.h"

#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QVector>
#include <QSoundEffect>

class Nivel3 : public Nivel
{
    Q_OBJECT
public:
    Nivel3(QGraphicsView* vista, QObject* parent = nullptr);
    void iniciarnivel() override;
    ~Nivel3();
    void detenerMusica() override;

private:
    QGraphicsView* vista;
    GokuNube* goku;
    int distanciaRecorrida;
    int vidas;
    QGraphicsTextItem* textoDistancia;
    QGraphicsPixmapItem* fondoItem;
    QSoundEffect efectoGolpe;
    QVector<QGraphicsRectItem*> barrasVida;

    QTimer* timerObstaculos;
    QTimer* timerDistancia;
    QTimer* timerScroll;

    void generarObstaculo();
    void actualizarDistancia();
    void perderVida();
    void mostrarGameOver();
    void actualizarScroll();
    QMediaPlayer* musicaNivel3 = nullptr;
    QAudioOutput* salidaAudio = nullptr;


};

#endif // NIVEL3_H
