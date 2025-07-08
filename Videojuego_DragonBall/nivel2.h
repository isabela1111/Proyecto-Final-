#ifndef NIVEL2_H
#define NIVEL2_H

#include "nivel.h"

#include "goku.h"
#include "taopaipaijefe.h"
#include <QGraphicsRectItem>
#include <QTimer>
#include <QMediaPlayer>


class Nivel2 : public Nivel {
    Q_OBJECT
public:
    explicit Nivel2(QGraphicsView* vista, QObject* parent = nullptr);
    void iniciarnivel() override;
    void actualizar() override;

    void mostrarPantallaGameOver();
    void mostrarPantallaVictoria();
    void crearBarrasVida();

private:
    Goku* goku;
    TaoPaiPaiJefe* taoPaiPai;
    QTimer* timerAtaques;

    QList<QGraphicsRectItem*> barrasVidaGoku;
    QList<QGraphicsRectItem*> barrasVidaTao;

    bool victoriaMostrada;
    bool derrotaMostrada;
    bool terminado;
    //void reiniciarEscena();
    void actualizarBarrasVida();
    QMediaPlayer* musicaNivel2;
    QAudioOutput* salidaAudio;



};

#endif // NIVEL2_H
