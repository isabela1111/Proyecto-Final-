#include "nivel2.h"
#include <QGraphicsPixmapItem>
#include <QDebug>
#include "recursos.h"

Nivel2::Nivel2(QObject* parent) : Nivel(parent) {
    escena = nullptr;
    goku = nullptr;
    taoPaiPai = nullptr;
    timerAtaques = nullptr;
    derrotaMostrada = false;
    victoriaMostrada = false;
    terminado = false;

    fondo.load(Recursos::fondoNivel2);
}

void Nivel2::iniciarnivel() {
    escena = new QGraphicsScene();
    escena->setSceneRect(0, 0, 800, 600);

    QGraphicsPixmapItem* fondoItem = new QGraphicsPixmapItem(fondo.scaled(800, 600));
    escena->addItem(fondoItem);

    goku = new Goku();
    taoPaiPai = new TaoPaiPaiJefe();
    taoPaiPai->setObjetivo(goku);

    goku->setPos(100, 400);
    taoPaiPai->setPos(600, 400);

    escena->addItem(goku);
    escena->addItem(taoPaiPai);

    personajes.append(goku);
    personajes.append(taoPaiPai);

    crearBarrasVida();

    timerAtaques = new QTimer(this);
    connect(timerAtaques, &QTimer::timeout, taoPaiPai, &TaoPaiPaiJefe::lanzarGranada);
    timerAtaques->start(3000);
}

void Nivel2::actualizar() {
    if (!goku || !taoPaiPai) return;

    actualizarBarrasVida();

    if (goku->getVida() <= 0 && !derrotaMostrada) {
        mostrarPantallaGameOver();
        derrotaMostrada = true;
        terminado = true;
    }

    if (taoPaiPai->getVida() <= 0 && !victoriaMostrada) {
        mostrarPantallaVictoria();
        victoriaMostrada = true;
        terminado = true;
    }
}

void Nivel2::crearBarrasVida() {
    for (QGraphicsRectItem* barra : barrasVidaGoku)
        delete barra;
    for (QGraphicsRectItem* barra : barrasVidaTao)
        delete barra;
    barrasVidaGoku.clear();
    barrasVidaTao.clear();

    for (int i = 0; i < goku->getVida(); ++i) {
        QGraphicsRectItem* barra = new QGraphicsRectItem(20 + i * 25, 20, 20, 20);
        barra->setBrush(Qt::green);
        escena->addItem(barra);
        barrasVidaGoku.append(barra);
    }

    for (int i = 0; i < taoPaiPai->getVida(); ++i) {
        QGraphicsRectItem* barra = new QGraphicsRectItem(580 + i * 25, 20, 20, 20);
        barra->setBrush(Qt::red);
        escena->addItem(barra);
        barrasVidaTao.append(barra);
    }
}

void Nivel2::actualizarBarrasVida() {
    for (int i = 0; i < barrasVidaGoku.size(); ++i) {
        barrasVidaGoku[i]->setVisible(i < goku->getVida());
    }

    for (int i = 0; i < barrasVidaTao.size(); ++i) {
        barrasVidaTao[i]->setVisible(i < taoPaiPai->getVida());
    }
}

void Nivel2::mostrarPantallaGameOver() {
    escena->clear();
    QPixmap fondoGameOver(Recursos::fondoGameOverGoku);
    if (fondoGameOver.isNull()) {
        qDebug() << "No se pudo cargar imagen de derrota.";
        return;
    }
    escena->addItem(new QGraphicsPixmapItem(fondoGameOver.scaled(800, 600)));
}

void Nivel2::mostrarPantallaVictoria() {
    escena->clear();
    QPixmap fondoVictoria(Recursos::fondoWinGoku);
    if (fondoVictoria.isNull()) {
        qDebug() << "No se pudo cargar imagen de victoria.";
        return;
    }
    escena->addItem(new QGraphicsPixmapItem(fondoVictoria.scaled(800, 600)));
}
