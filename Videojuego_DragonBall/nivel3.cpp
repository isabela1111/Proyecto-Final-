#include "nivel3.h"
#include "recursos.h"
#include <QRandomGenerator>
#include <QFont>

Nivel3::Nivel3(QGraphicsView* vista, QObject* parent)
    : Nivel(parent), vista(vista), distanciaRecorrida(0), vidas(5)
{
    escena = new QGraphicsScene();
    escena->setSceneRect(0, 0, 800, 600);
    fondo.load(Recursos::fondoNivel3);
    terminado = false;
}

void Nivel3::iniciarnivel() {
    escena->setBackgroundBrush(QBrush(fondo.scaled(800, 600)));

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
}

void Nivel3::generarObstaculo() {
    AvionEnemigo* avion = new AvionEnemigo(goku);
    escena->addItem(avion);
    avion->setPos(800, QRandomGenerator::global()->bounded(0, 550));
}

void Nivel3::actualizarDistancia() {
    distanciaRecorrida += 10;
    textoDistancia->setPlainText("Distancia: " + QString::number(distanciaRecorrida) + " m");

    if (distanciaRecorrida >= 300) {
        QGraphicsTextItem* victoria = escena->addText("¡Llegaste a la base!");
        victoria->setDefaultTextColor(Qt::yellow);
        victoria->setFont(QFont("Arial", 30, QFont::Bold));
        victoria->setPos(250, 250);
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
    QPixmap gameOverPixmap(Recursos::fondoGameOverGoku);
    QGraphicsPixmapItem* gameOver = escena->addPixmap(gameOverPixmap.scaled(300, 150));
    gameOver->setPos(250, 200);
    terminado = true;
}

void Nivel3::actualizar() {
    QList<QGraphicsItem*> items = escena->items();
    for (QGraphicsItem* item : items) {
        AvionEnemigo* avion = dynamic_cast<AvionEnemigo*>(item);
        if (avion && avion->collidesWithItem(goku)) {
            perderVida();
            escena->removeItem(avion);
            delete avion;
            break;
        }
    }
}
