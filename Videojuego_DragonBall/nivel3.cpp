#include "nivel3.h"
#include "recursos.h"
#include "avionenemigo.h"
#include "gokunube.h"
#include "misil.h"

#include <QRandomGenerator>
#include <QFont>
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QCoreApplication>

Nivel3::Nivel3(QGraphicsView* vista, QObject* parent)
    : Nivel(vista, parent), vista(vista), distanciaRecorrida(0), vidas(5)
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
    connect(goku, &GokuNube::gokuRecibeDanio, this, &Nivel3::perderVida);

    timerObstaculos->start(800);

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
    if (!goku || terminado) return;
    int altoEscena = static_cast<int>(escena->sceneRect().height());
    int altoAvion = 43;
    int margen = 20;
    int y = QRandomGenerator::global()->bounded(margen, altoEscena - altoAvion - margen);
    AvionEnemigo* avion = new AvionEnemigo(goku);
    float dificultad = distanciaRecorrida / 150.0;
    if (dificultad > 2.0) dificultad = 2.0;
    avion->setVelocidad(2.5 + dificultad * 1.2);
    avion->setMaxMisiles(1 + dificultad * 2);
    escena->addItem(avion);
    avion->setPos(goku->x() + 800, y);
    // Conexiones
    connect(avion, &AvionEnemigo::colisionaConGoku, this, &Nivel3::perderVida);
    connect(avion, &AvionEnemigo::disparoMisil, escena, &QGraphicsScene::addItem);
    int nuevaFrecuencia = 1500 - (distanciaRecorrida / 10);
    if (nuevaFrecuencia < 500) nuevaFrecuencia = 500;
    timerObstaculos->start(nuevaFrecuencia);
}

void Nivel3::actualizarDistancia() {
    if (terminado || !goku) return;
    distanciaRecorrida += 10;
    textoDistancia->setPlainText("Distancia: " + QString::number(distanciaRecorrida) + " m");
    if (distanciaRecorrida >= 1000) {
        terminado = true;
        if (timerObstaculos) { timerObstaculos->stop(); delete timerObstaculos; timerObstaculos = nullptr; }
        if (timerDistancia) { timerDistancia->stop(); delete timerDistancia; timerDistancia = nullptr; }
        if (timerScroll)    { timerScroll->stop(); delete timerScroll; timerScroll = nullptr; }
        if (goku) {
            goku->blockSignals(true);
            goku->setEnabled(false);
            goku->setVisible(false);
            escena->removeItem(goku);
            goku->deleteLater();
            goku = nullptr;
        }
        QPixmap winPixmap(Recursos::fondoWinGoku);
        if (winPixmap.isNull()) {
            qDebug() << "Error: no se pudo cargar fondoWinGoku";
            return;
        }
        QGraphicsPixmapItem* winItem = escena->addPixmap(winPixmap.scaled(800, 600));
        winItem->setZValue(100);
        QPointF centroVista = vista->mapToScene(vista->viewport()->rect().center());
        winItem->setPos(centroVista.x() - 400, centroVista.y() - 300);
        // Botón Volver al menu
        QPushButton* botonMenu = new QPushButton("Volver al menú");
        botonMenu->setFixedSize(200, 50);
        botonMenu->setStyleSheet("background-color: white; color: black; font-weight: bold; border-radius: 10px;");
        QGraphicsProxyWidget* proxy = escena->addWidget(botonMenu);
        proxy->setZValue(101);
        proxy->setPos(centroVista.x() - 100, centroVista.y() + 150);
        connect(botonMenu, &QPushButton::clicked, [this]() {
            this->vista->close();
        });
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
    if (timerScroll)    { timerScroll->stop(); delete timerScroll; timerScroll = nullptr; }
    for (QGraphicsItem* item : escena->items()) {
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
    QGraphicsPixmapItem* gameOver = escena->addPixmap(gameOverPixmap.scaled(800, 600));
    gameOver->setZValue(100);
    QPointF centroVista = vista->mapToScene(vista->viewport()->rect().center());
    gameOver->setPos(centroVista.x() - 400, centroVista.y() - 300);
    // Boton para volver al menu
    QPushButton* botonMenu = new QPushButton("Volver al menú");
    botonMenu->setFixedSize(200, 50);
    botonMenu->setStyleSheet("background-color: white; color: black; border-radius: 10px;");
    QGraphicsProxyWidget* proxyBoton = escena->addWidget(botonMenu);
    proxyBoton->setZValue(101);
    proxyBoton->setPos(centroVista.x() - 100, centroVista.y() + 150);
    connect(botonMenu, &QPushButton::clicked, [this]() {
        this->vista->close(); });
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
