#include "nivel1.h"
#include "recursos.h"
#include "piedra.h"
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QDebug>
#include <QFont>

Nivel1::Nivel1(QGraphicsView* vista, QObject* parent)
    : Nivel(parent), vista(vista)
{
    escena = new QGraphicsScene();
    escena->setSceneRect(0, 0, 800, 600);
    vista->setScene(escena);

    alturaActual = 0;
    alturaMeta = 600;

    vidasTotales = 5;
    vidasRestantes = vidasTotales;
    gameOverShown = false;
    juegoIniciado = false;

    crearBarrasVida();
    temporizador = nullptr;
    timerCronometro = nullptr;
    timerPiedras = nullptr;

    textoCronometro = new QGraphicsTextItem();
    textoCronometro->setDefaultTextColor(Qt::black);
    textoCronometro->setFont(QFont("Consolas", 30, QFont::Bold));
    textoCronometro->setPos(500, 10);
    textoCronometro->setZValue(2);
    escena->addItem(textoCronometro);
    tiempoRestante = 3 * 60;
}

void Nivel1::iniciarnivel() {
    fondo.load(Recursos::fondoNivel1);
    if (fondo.isNull()) {
        qDebug() << "No se pudo cargar el fondo del nivel 1.";
        return;}
    QPixmap fondoEscalado = fondo.scaled(800, 600);
    escena->addPixmap(fondoEscalado);
    crearBarrasVida();
    escena->addItem(textoCronometro);
    taoPaiPai = new TaoPaiPai(vista, this);
    escena->addItem(taoPaiPai);
    taoPaiPai->setPos(250, 500);

}

void Nivel1::comenzarJuego() {
    if (juegoIniciado || gameOverShown) return;
    juegoIniciado = true;

    temporizador = new QTimer(this);
    connect(temporizador, &QTimer::timeout, this, [=]() {
        if (!gameOverShown) {
            verificarCaida();
            aplicarGravedad();
        }
    });
    temporizador->start(30);
    timerCronometro = new QTimer(this);
    connect(timerCronometro, &QTimer::timeout, this, &Nivel1::actualizarCronometro);
    timerCronometro->start(3000);

    timerPiedras = new QTimer(this);
    connect(timerPiedras, &QTimer::timeout, this, [=]() {
        Piedra* piedra = new Piedra(this);
        piedra->setPos(305, 0);
        escena->addItem(piedra);});
    timerPiedras->start(3000);
 }

void Nivel1::verificarCaida() {
    if (!taoPaiPai->cayendo && taoPaiPai->y() + taoPaiPai->boundingRect().height() > escena->height()) {
        taoPaiPai->cayendo = true;

        if (vidasRestantes > 0) {
            vidasRestantes--;
            if (vidasRestantes >= 0 && vidasRestantes < barrasVida.size()) {
                QGraphicsRectItem* barra = barrasVida.takeAt(vidasRestantes);
                if (barra) {
                    escena->removeItem(barra);
                    delete barra;
                }
            }
            taoPaiPai->mostrarCaida();
            QTimer::singleShot(500, this, [this]() {
                taoPaiPai->setPos(250, 500);
                taoPaiPai->cayendo = false;
            });
        }
        if (vidasRestantes <= 0 && !gameOverShown) {
            QTimer::singleShot(600, this, [this]() {
                mostrarPantallaGameOver();
            });
        }
    }
}

void Nivel1::aplicarGravedad() {
    if (!gameOverShown && juegoIniciado && !taoPaiPai->cayendo &&
        taoPaiPai->y() + taoPaiPai->boundingRect().height() < escena->height()){
        taoPaiPai->moveBy(0, 3);
    }
}

void Nivel1::actualizarCronometro() {
    if (gameOverShown || !juegoIniciado) return;
    tiempoRestante--;
    int minutos = tiempoRestante / 60;
    int segundos = tiempoRestante % 60;
    textoCronometro->setPlainText(QString("Tiempo: %1:%2")
                                      .arg(minutos, 2, 10, QLatin1Char('0'))
                                      .arg(segundos, 2, 10, QLatin1Char('0')));
    if (tiempoRestante <= 0) {
        if (vidasRestantes > 0) {
            mostrarPantallaVictoria();
        }
        else {
            mostrarPantallaGameOver();
        }
    }
}

void Nivel1::mostrarPantallaGameOver() {
    gameOverShown = true;

    if (temporizador) temporizador->stop();
    if (timerCronometro) timerCronometro->stop();
    if (timerPiedras) timerPiedras->stop();
    escena->clear();
    QPixmap fondoGameOver(Recursos::fondoGameOverTao);
    if (fondoGameOver.isNull()) {
        qDebug() << "No se pudo cargar fondoGameOverTao.";
        return;
    }
    QGraphicsPixmapItem* fondoItem = new QGraphicsPixmapItem(fondoGameOver.scaled(800, 600));
    escena->addItem(fondoItem);
}

void Nivel1::mostrarPantallaVictoria() {
    gameOverShown = true;
    escena->clear();
    QPixmap fondoWin(Recursos::fondoWinTao);
    if (fondoWin.isNull()) {
        qDebug() << "No se pudo cargar fondoWinTao.";
        return;
    }
    QGraphicsPixmapItem* fondoItem = new QGraphicsPixmapItem(fondoWin.scaled(800, 600));
    escena->addItem(fondoItem);
}

void Nivel1::crearBarrasVida() {
    for (QGraphicsRectItem* barra : barrasVida) {
        if (barra) {
            escena->removeItem(barra);
            delete barra;
        }
    }
    barrasVida.clear();
    for (int i = 0; i < vidasTotales; ++i) {
        QGraphicsRectItem* barra = new QGraphicsRectItem(10 + i * 25, 10, 20, 20);
        barra->setBrush(Qt::green);
        barra->setZValue(2);
        escena->addItem(barra);
        barrasVida.append(barra);
    }
}

void Nivel1::quitarVida(){
    if (vidasRestantes > 0) {
        vidasRestantes--;
        if (vidasRestantes >= 0 && vidasRestantes < barrasVida.size()) {
            QGraphicsRectItem* barra = barrasVida.takeAt(vidasRestantes);
            if (barra) {
                escena->removeItem(barra);
                delete barra;}
        }
        if (vidasRestantes <= 0 && !gameOverShown) {
            QTimer::singleShot(200, this, [this]() {
                mostrarPantallaGameOver();});
        }
    }
}

