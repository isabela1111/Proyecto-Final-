#include "nivel1.h"
#include "recursos.h"
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QDebug>
#include <QFont> //Estilo de letra.

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

    textoCronometro = new QGraphicsTextItem();
    textoCronometro->setDefaultTextColor(Qt::black);
    textoCronometro->setFont(QFont("Consolas", 30, QFont::Bold));
    textoCronometro->setPos(500, 10);
    textoCronometro->setZValue(2);
    escena->addItem(textoCronometro);
    tiempoRestante = 5 * 60;
}

void Nivel1::iniciarnivel() {
    fondo.load(Recursos::fondoNivel1);
    if (fondo.isNull()) {
        qDebug() << "No se pudo cargar el fondo del nivel 1.";
        return;
    }
    QPixmap fondoEscalado = fondo.scaled(800, 600);
    escena->addPixmap(fondoEscalado);

    crearBarrasVida();
    escena->addItem(textoCronometro);

    taoPaiPai = new TaoPaiPai(vista, this);
    escena->addItem(taoPaiPai);
    taoPaiPai->setPos(250, 500);
    comenzarJuego();
}

void Nivel1::comenzarJuego() {
    if (juegoIniciado || gameOverShown) return;
    juegoIniciado = true;

    // Timer del juego
    QTimer* temporizador = new QTimer(this);
    connect(temporizador, &QTimer::timeout, this, [=]() {
        if (!gameOverShown) {
            verificarCaida();
            aplicarGravedad();
        }
    });
    temporizador->start(30);

    // Timer del cronómetro
    QTimer* timerCronometro = new QTimer(this);
    connect(timerCronometro, &QTimer::timeout, this, &Nivel1::actualizarCronometro);
    timerCronometro->start(1000);
}

void Nivel1::verificarCaida() {
    if (taoPaiPai->y() > escena->height()) {
        if (vidasRestantes > 0) {
            vidasRestantes--;

            if (vidasRestantes >= 0 && vidasRestantes < barrasVida.size()) {
                QGraphicsRectItem* barra = barrasVida[vidasRestantes];
                if (barra) {
                    escena->removeItem(barra);
                    delete barra;
                    barrasVida[vidasRestantes] = nullptr;
                }
            }
            taoPaiPai->setPos(250, 500);
        }
        if (vidasRestantes <= 0 && !gameOverShown) {
            mostrarPantallaGameOver();
        }
    }
}

void Nivel1::aplicarGravedad() {
    if (!gameOverShown && juegoIniciado &&
        taoPaiPai->y() + taoPaiPai->boundingRect().height() < escena->height()) {
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
        escena->removeItem(barra);
        delete barra;
    }
    barrasVida.clear();
    for (int i = 0; i < vidasTotales; ++i) {
        QGraphicsRectItem* barra = new QGraphicsRectItem(10 + i * 25, 10, 20, 20);
        barra->setBrush(Qt::green);
        barra->setZValue(2);
        barrasVida.append(barra);
        escena->addItem(barra);
    }
}
