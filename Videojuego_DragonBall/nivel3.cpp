#include "nivel3.h"
#include "recursos.h"
#include "avionenemigo.h"
#include "gokunube.h"

#include <QRandomGenerator>
#include <QFont>
#include <QGraphicsPixmapItem>

Nivel3::Nivel3(QGraphicsView* vista, QObject* parent)
    : Nivel(parent), vista(vista), distanciaRecorrida(0), vidas(5)
{
    escena = new QGraphicsScene();
    escena->setSceneRect(0, 0, 8000, 600);
    fondo.load(Recursos::fondoNivel3);
    terminado = false;
}

void Nivel3::iniciarnivel() {
    fondoItem = new QGraphicsPixmapItem(fondo.scaled(8000, 600));
    escena->addItem(fondoItem);

    goku = new GokuNube(vista);
    escena->addItem(goku);
    goku->setPos(100, 300);
    goku->setFocus();

    textoDistancia = new QGraphicsTextItem();
    textoDistancia->setDefaultTextColor(Qt::white);
    textoDistancia->setFont(QFont("Arial", 16));
    textoDistancia->setPos(10, 10);
    escena->addItem(textoDistancia);
    actualizarDistancia();

    for (int i = 0; i < vidas; ++i) {
        QGraphicsRectItem* barra = new QGraphicsRectItem(0, 0, 25, 10);
        barra->setBrush(Qt::green);
        barra->setPos(10 + i * 30, 40);
        escena->addItem(barra);
        barrasVida.append(barra);
    }

    timerObstaculos = new QTimer(this);
    connect(timerObstaculos, &QTimer::timeout, this, &Nivel3::generarObstaculo);
    timerObstaculos->start(1500);

    timerDistancia = new QTimer(this);
    connect(timerDistancia, &QTimer::timeout, this, &Nivel3::actualizarDistancia);
    timerDistancia->start(1000);

    vista->setScene(escena);
    vista->centerOn(goku);

    timerScroll = new QTimer(this);
    connect(timerScroll, &QTimer::timeout, this, &Nivel3::actualizarScroll);
    timerScroll->start(30);
}

void Nivel3::generarObstaculo() {
    AvionEnemigo* avion = new AvionEnemigo(goku);
    escena->addItem(avion);
    avion->setPos(goku->x() + 800, QRandomGenerator::global()->bounded(0, 550));
    connect(avion, &AvionEnemigo::colisionaConGoku, this, &Nivel3::perderVida);
}

void Nivel3::actualizarDistancia() {
    distanciaRecorrida += 10;
    textoDistancia->setPlainText("Distancia: " + QString::number(distanciaRecorrida) + " m");

    if (distanciaRecorrida >= 1000) {
        QGraphicsTextItem* victoria = escena->addText("¡Llegaste a la base!");
        victoria->setDefaultTextColor(Qt::yellow);
        victoria->setFont(QFont("Arial", 30, QFont::Bold));
        victoria->setPos(goku->x() - 200, 250);
        terminado = true;
    }
}

void Nivel3::perderVida() {
    if (vidas > 0) {
        vidas--;
        QGraphicsRectItem* barra = barrasVida.takeLast();
        escena->removeItem(barra);
        delete barra;
    }
    if (vidas <= 0) {
        mostrarGameOver();
    }
}

void Nivel3::mostrarGameOver() {
    timerObstaculos->stop();
    timerDistancia->stop();
    timerScroll->stop();
    escena->removeItem(goku);
    delete goku;
    goku = nullptr;
    QList<QGraphicsItem*> items = escena->items();
    for (QGraphicsItem* item : items) {
        AvionEnemigo* avion = dynamic_cast<AvionEnemigo*>(item);
        if (avion) {
            escena->removeItem(avion);
            delete avion;
        }
    }
    QPixmap gameOverPixmap(Recursos::fondoGameOverGoku);
    QGraphicsPixmapItem* gameOver = escena->addPixmap(gameOverPixmap.scaled(800, 600));
    gameOver->setZValue(10);
    gameOver->setPos(vista->mapToScene(0, 0));
    terminado = true;
}

void Nivel3::actualizarScroll() {
    if (terminado || goku == nullptr) return;
    vista->centerOn(goku);
    QPointF vistaCentro = vista->mapToScene(vista->viewport()->rect().topLeft());
    textoDistancia->setPos(vistaCentro.x() + 10, vistaCentro.y() + 10);
    for (int i = 0; i < barrasVida.size(); ++i) {
        barrasVida[i]->setPos(vistaCentro.x() + 10 + i * 30, vistaCentro.y() + 40);
    }
}


void Nivel3::actualizar() {
}
