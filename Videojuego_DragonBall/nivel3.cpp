#include "nivel3.h"
#include "recursos.h"
#include "avionenemigo.h"
#include "gokunube.h"
#include "misil.h"

#include <QRandomGenerator>
#include <QFont>
#include <QGraphicsPixmapItem>
#include <QDebug>

Nivel3::Nivel3(QGraphicsView* vista, QObject* parent)
    : Nivel(parent), vista(vista), distanciaRecorrida(0), vidas(5)
{
    escena = new QGraphicsScene();
    escena->setSceneRect(0, 0, 8000, 600);
    fondo.load(Recursos::fondoNivel3);
    terminado = false;

    efectoGolpe.setSource(QUrl::fromLocalFile(Recursos::sonidoGolpe));
    efectoGolpe.setVolume(1.0);
}

Nivel3::~Nivel3() {
    if (timerObstaculos) { timerObstaculos->stop(); delete timerObstaculos; timerObstaculos = nullptr; }
    if (timerDistancia) { timerDistancia->stop(); delete timerDistancia; timerDistancia = nullptr; }
    if (timerScroll)    { timerScroll->stop(); delete timerScroll; timerScroll = nullptr; }
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

    timerScroll = new QTimer(this);
    connect(timerScroll, &QTimer::timeout, this, &Nivel3::actualizarScroll);
    timerScroll->start(30);

    vista->setScene(escena);
    vista->centerOn(goku);
}

void Nivel3::generarObstaculo() {
    AvionEnemigo* avion = new AvionEnemigo(goku);
    escena->addItem(avion);
    avion->setPos(goku->x() + 800, QRandomGenerator::global()->bounded(0, 550));
    connect(avion, &AvionEnemigo::colisionaConGoku, this, &Nivel3::perderVida);
    
    // Conexión para agregar los misiles disparados a la escena
    connect(avion, &AvionEnemigo::disparoMisil, escena, &QGraphicsScene::addItem);
}

void Nivel3::actualizarDistancia() {
    if (terminado || !goku) return;

    distanciaRecorrida += 10;
    textoDistancia->setPlainText("Distancia: " + QString::number(distanciaRecorrida) + " m");

    if (distanciaRecorrida >= 1000) {
        QGraphicsTextItem* victoria = escena->addText("\u00a1Llegaste a la base!");
        victoria->setDefaultTextColor(Qt::yellow);
        victoria->setFont(QFont("Arial", 30, QFont::Bold));
        victoria->setPos(goku->x() - 200, 250);
        terminado = true;

        if (timerObstaculos) timerObstaculos->stop();
        if (timerDistancia) timerDistancia->stop();
        if (timerScroll) timerScroll->stop();
    }
}

void Nivel3::perderVida() {
    if (terminado) return;

    efectoGolpe.play();
    if (vidas > 0) {
        vidas--;
        QGraphicsRectItem* barra = barrasVida.takeLast();
        escena->removeItem(barra);
        delete barra;
    }

    if (vidas <= 0 && !terminado) {
        mostrarGameOver();
    }
}

void Nivel3::mostrarGameOver() {
    if (terminado) return;
    terminado = true;

    if (timerObstaculos) { timerObstaculos->stop(); delete timerObstaculos; timerObstaculos = nullptr; }
    if (timerDistancia) { timerDistancia->stop(); delete timerDistancia; timerDistancia = nullptr; }
    if (timerScroll) { timerScroll->stop(); delete timerScroll; timerScroll = nullptr; }

    // Eliminar enemigos
    QList<QGraphicsItem*> items = escena->items();
    for (QGraphicsItem* item : items) {
        AvionEnemigo* avion = dynamic_cast<AvionEnemigo*>(item);
        if (avion) {
            avion->scene()->removeItem(avion);
            avion->deleteLater();
        }
    }

    if (goku) {
        goku->blockSignals(true);
        goku->setEnabled(false);
        goku->setVisible(false);
        escena->removeItem(goku);
        goku->deleteLater();
        goku = nullptr;
    }

    QPixmap gameOverPixmap(Recursos::fondoGameOverGoku);
    if (gameOverPixmap.isNull()) {
        qDebug() << "Error: no se pudo cargar fondoGameOverGoku";
        return;
    }

    QGraphicsPixmapItem* gameOver = escena->addPixmap(gameOverPixmap.scaled(800, 600));
    gameOver->setZValue(100);
    QPointF centroVista = vista->mapToScene(vista->viewport()->rect().center());
    gameOver->setPos(centroVista.x() - 400, centroVista.y() - 300);
}

void Nivel3::actualizarScroll() {
    if (terminado || !goku) return;
    vista->centerOn(goku);
    QPointF vistaCentro = vista->mapToScene(vista->viewport()->rect().topLeft());

    textoDistancia->setPos(vistaCentro.x() + 10, vistaCentro.y() + 10);

    for (int i = 0; i < barrasVida.size(); ++i) {
        barrasVida[i]->setPos(vistaCentro.x() + 10 + i * 30, vistaCentro.y() + 40);
    }
}
