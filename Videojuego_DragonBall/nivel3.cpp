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
#include <QFile>
#include <stdexcept>

Nivel3::Nivel3(QGraphicsView* vista, QObject* parent)
    : Nivel(vista, parent),
    vista(vista),distanciaRecorrida(0),vidas(5),musicaNivel3(nullptr),salidaAudio(nullptr),cooldownMisil(nullptr),
    puedeLanzarMisil(true),misilDisponible(false),avisoMisil(nullptr),temporizadorMisilEspecial(new QTimer(this))
{
    escena = new QGraphicsScene();
    escena->setSceneRect(0, 0, 8000, 600);
    fondo.load(Recursos::fondoNivel3);
    terminado = false;
     efectoGolpe.setSource(QUrl::fromLocalFile(Recursos::sonidoGolpe));
    efectoGolpe.setVolume(1.0);
    temporizadorMisilEspecial->setSingleShot(true);
    connect(temporizadorMisilEspecial, &QTimer::timeout, [=]() {
        misilDisponible = true;
        if (avisoMisil)
            avisoMisil->setPlainText("¡Misil listo! Presiona ESPACIO");
    });
}

Nivel3::~Nivel3() {
    if (timerObstaculos) { timerObstaculos->stop(); delete timerObstaculos; timerObstaculos = nullptr; }
    if (timerDistancia) { timerDistancia->stop(); delete timerDistancia; timerDistancia = nullptr; }
    if (timerScroll)    { timerScroll->stop(); delete timerScroll; timerScroll = nullptr; }
}

void Nivel3::iniciarnivel() {
    try {
        fondoItem = new QGraphicsPixmapItem(fondo.scaled(8000, 600));
        escena->addItem(fondoItem);
        goku = new GokuNube(vista);
        escena->addItem(goku);
        goku->setPos(100, 300);
        goku->setFocus();
        connect(goku, &GokuNube::lanzarSuperMisil, this, [this]() {
            if (puedeLanzarMisil && !terminado) {
                puedeLanzarMisil = false;
                lanzarSuperMisilGoku();
                if (!cooldownMisil) {
                    cooldownMisil = new QTimer(this);
                    cooldownMisil->setSingleShot(true);
                    connect(cooldownMisil, &QTimer::timeout, [this]() {
                        puedeLanzarMisil = true;
                    });
                }
                cooldownMisil->start(5000); // 5 segundos
            }
        });
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
        if (!QFile::exists(Recursos::sonidoNivel3)) {
            throw std::runtime_error("Archivo de sonido de Nivel 3 no encontrado.");
        }
        musicaNivel3 = new QMediaPlayer(this);
        salidaAudio = new QAudioOutput(this);
        musicaNivel3->setAudioOutput(salidaAudio);
        salidaAudio->setVolume(0.2);
        musicaNivel3->setSource(QUrl("qrc" + Recursos::sonidoNivel3));
        musicaNivel3->play();
        avisoMisil = new QGraphicsTextItem("Cargando misil...");
        avisoMisil->setDefaultTextColor(Qt::yellow);
        avisoMisil->setFont(QFont("Arial", 14, QFont::Bold));
        avisoMisil->setZValue(10);
        avisoMisil->setPos(10, 70);  // Posición inicial
        escena->addItem(avisoMisil);
        temporizadorMisilEspecial->start(5000);
        vista->setScene(escena);
        vista->centerOn(goku);
    }
    catch (const std::exception& e) {
        qDebug() << "Error al iniciar Nivel 3:" << e.what();
    }
}

void Nivel3::generarObstaculo() {
    if (!goku || terminado) return;

    int altoEscena = static_cast<int>(escena->sceneRect().height());
    int altoAvion = 43;
    int margen = 20;
    int cantidadAviones = 1 + distanciaRecorrida / 200;
    if (cantidadAviones > 5) cantidadAviones = 5;
    for (int i = 0; i < cantidadAviones; ++i) {
        int y = QRandomGenerator::global()->bounded(margen, altoEscena - altoAvion - margen);
        AvionEnemigo* avion = new AvionEnemigo(goku);

        float dificultad = distanciaRecorrida / 150.0;
        if (dificultad > 2.0) dificultad = 2.0;

        avion->setVelocidad(2.5 + dificultad * 1.2);
        avion->setMaxMisiles(1 + dificultad * 2);
        escena->addItem(avion);
        avion->setPos(goku->x() + 800 + i * 100, y);

        connect(avion, &AvionEnemigo::colisionaConGoku, this, &Nivel3::perderVida);
        connect(avion, &AvionEnemigo::disparoMisil, escena, &QGraphicsScene::addItem);
    }
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
    try {
        if (terminado) return;
        terminado = true;
        if (timerObstaculos) { timerObstaculos->stop(); delete timerObstaculos; timerObstaculos = nullptr; }
        if (timerDistancia) { timerDistancia->stop(); delete timerDistancia; timerDistancia = nullptr; }
        if (timerScroll)    { timerScroll->stop(); delete timerScroll; timerScroll = nullptr; }
        if (musicaNivel3) musicaNivel3->stop();
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
        if (gameOverPixmap.isNull()) throw std::runtime_error("Fondo Game Over no pudo cargarse.");
        QGraphicsPixmapItem* gameOver = escena->addPixmap(gameOverPixmap.scaled(800, 600));
        gameOver->setZValue(100);
        QPointF centroVista = vista->mapToScene(vista->viewport()->rect().center());
        gameOver->setPos(centroVista.x() - 400, centroVista.y() - 300);

        QPushButton* botonMenu = new QPushButton("Volver al menú");
        botonMenu->setFixedSize(200, 50);
        botonMenu->setStyleSheet("background-color: white; color: black; border-radius: 10px;");
        QGraphicsProxyWidget* proxyBoton = escena->addWidget(botonMenu);
        proxyBoton->setZValue(101);
        proxyBoton->setPos(centroVista.x() - 100, centroVista.y() + 150);
        connect(botonMenu, &QPushButton::clicked, [this]() { this->vista->close(); });
    }
    catch (const std::exception& e) {
        qDebug() << "Error al mostrar Game Over:" << e.what();
    }
}

void Nivel3::actualizarScroll() {
    if (terminado || !goku) return;

    vista->centerOn(goku);
    QPointF vistaCentro = vista->mapToScene(vista->viewport()->rect().topLeft());

    textoDistancia->setPos(vistaCentro.x() + 10, vistaCentro.y() + 10);

    for (int i = 0; i < barrasVida.size(); ++i) {
        barrasVida[i]->setPos(vistaCentro.x() + 10 + i * 30, vistaCentro.y() + 40);
    }

    if (avisoMisil) {
        avisoMisil->setPos(vistaCentro.x() + 10, vistaCentro.y() + 70);
    }
}


void Nivel3::detenerMusica() {
    if (musicaNivel3 && musicaNivel3->isPlaying()) {
        musicaNivel3->stop();
        qDebug() << "Musica nivel 3 detenida.";
    }
}

void Nivel3::lanzarSuperMisilGoku() {
    QGraphicsPixmapItem* misil = new QGraphicsPixmapItem(QPixmap(Recursos::misilSprite).scaled(40, 20));
    misil->setZValue(3);
    misil->setPos(goku->x() + goku->boundingRect().width(), goku->y() + 10);
    escena->addItem(misil);

    QTimer* moverTimer = new QTimer(this);
    connect(moverTimer, &QTimer::timeout, [=]() {
        misil->setX(misil->x() + 10);
        if (misil->x() > escena->sceneRect().right()) {
            escena->removeItem(misil);
            delete misil;
            moverTimer->stop();
            moverTimer->deleteLater();
        } else {
            QList<QGraphicsItem*> colisiones = misil->collidingItems();
            for (QGraphicsItem* item : colisiones) {
                AvionEnemigo* avion = dynamic_cast<AvionEnemigo*>(item);
                if (avion) {
                    escena->removeItem(avion);
                    avion->deleteLater();
                }
            }
        }
    });
    moverTimer->start(30);
    avisoMisil = new QGraphicsTextItem("Cargando misil...");
    avisoMisil->setDefaultTextColor(Qt::yellow);
    avisoMisil->setFont(QFont("Arial", 14, QFont::Bold));
    avisoMisil->setZValue(10);
    avisoMisil->setPos(10, 70);
    escena->addItem(avisoMisil);
    temporizadorMisilEspecial->start(5000);

}
