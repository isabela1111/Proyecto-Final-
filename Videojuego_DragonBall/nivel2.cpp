#include "nivel2.h"
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QGraphicsProxyWidget>
#include <QPushButton>
#include <QTimer>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "recursos.h"
#include "goku.h"
#include "taopaipaijefe.h"

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
    vista->setScene(escena);

    QGraphicsPixmapItem* fondoItem = new QGraphicsPixmapItem(fondo.scaled(800, 600));
    escena->addItem(fondoItem);
    if (!goku) {
        goku = new Goku(vista);
        connect(goku, &Goku::danioRecibido, this, [this]() {
            qDebug() << "Goku recibió daño → se reinicia la escena";
            //reiniciarEscena();
        });
    }
    if (!taoPaiPai) {
        taoPaiPai = new TaoPaiPaiJefe(vista);
        taoPaiPai->setObjetivo(goku);
    }
    goku->setPos(100, 400);
    taoPaiPai->setPos(600, 400);
    escena->addItem(goku);
    escena->addItem(taoPaiPai);
    goku->setFocus();
    if (personajes.isEmpty()) {
        personajes.append(goku);
        personajes.append(taoPaiPai);
    }
    if (barrasVidaGoku.isEmpty() && barrasVidaTao.isEmpty()) {
        crearBarrasVida();
    } else {
        for (auto* barra : barrasVidaGoku) escena->addItem(barra);
        for (auto* barra : barrasVidaTao) escena->addItem(barra);
    }
    if (!timerAtaques) {
        timerAtaques = new QTimer(this);
        connect(timerAtaques, &QTimer::timeout, taoPaiPai, &TaoPaiPaiJefe::lanzarGranada);
        timerAtaques->start(3000);
    }
    QTimer* timerActualizacion = new QTimer(this);
    connect(timerActualizacion, &QTimer::timeout, this, &Nivel2::actualizar);
    timerActualizacion->start(100);
    musicaNivel2 = new QMediaPlayer(this);
    salidaAudio = new QAudioOutput(this);

    musicaNivel2->setAudioOutput(salidaAudio);
    salidaAudio->setVolume(0.3);  // volumen entre 0.0 y 1.0

    musicaNivel2->setSource(QUrl("qrc:/Recursos/Sonidos/Nivel2.wav"));
    musicaNivel2->play();

}

/*
void Nivel2::reiniciarEscena() {
    if (!escena) return;

    escena->clear();

    QGraphicsPixmapItem* fondoItem = new QGraphicsPixmapItem(fondo.scaled(800, 600));
    escena->addItem(fondoItem);

    // Reposicionar personajes existentes
    goku->setPos(100, 400);
    taoPaiPai->setPos(600, 400);
    escena->addItem(goku);
    escena->addItem(taoPaiPai);

    // Volver a mostrar barras de vida
    for (auto* barra : barrasVidaGoku)
        escena->addItem(barra);
    for (auto* barra : barrasVidaTao)
        escena->addItem(barra);

    goku->setFocus();
}
*/


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
    if (musicaNivel2) {
        musicaNivel2->stop();
    }
    escena->clear();
    QPixmap fondoGameOver(Recursos::fondoGameOverGoku);
    if (fondoGameOver.isNull()) return;

    QGraphicsPixmapItem* fondoItem = new QGraphicsPixmapItem(fondoGameOver.scaled(800, 600));
    fondoItem->setZValue(0);
    escena->addItem(fondoItem);
    fondoItem->setPos(0, 0);

    QPushButton* botonMenu = new QPushButton("Volver al menú");
    botonMenu->setFixedSize(200, 50);
    botonMenu->setStyleSheet("background-color: white; color: black; font-weight: bold; border-radius: 10px;");
    QGraphicsProxyWidget* proxy = escena->addWidget(botonMenu);
    proxy->setZValue(1);
    QTimer::singleShot(30, this, [this, proxy]() {
        QPointF centroVista = vista->mapToScene(vista->viewport()->rect().center());
        proxy->setPos(centroVista.x() - 100, centroVista.y() + 150);
    });
    connect(botonMenu, &QPushButton::clicked, this, &Nivel2::volverAlMenu);
}

void Nivel2::mostrarPantallaVictoria() {
    if (musicaNivel2) {
        musicaNivel2->stop();
    }

    escena->clear();
    QPixmap fondoVictoria(Recursos::fondoWinGoku);
    if (fondoVictoria.isNull()) return;

    QGraphicsPixmapItem* fondoItem = new QGraphicsPixmapItem(fondoVictoria.scaled(800, 600));
    fondoItem->setZValue(0);
    escena->addItem(fondoItem);
    fondoItem->setPos(0, 0);

    QPushButton* botonMenu = new QPushButton("Volver al menú");
    botonMenu->setFixedSize(200, 50);
    botonMenu->setStyleSheet("background-color: white; color: black; font-weight: bold; border-radius: 10px;");
    QGraphicsProxyWidget* proxy = escena->addWidget(botonMenu);
    proxy->setZValue(1);
    QTimer::singleShot(30, this, [this, proxy]() {
        QPointF centroVista = vista->mapToScene(vista->viewport()->rect().center());
        proxy->setPos(centroVista.x() - 100, centroVista.y() + 150);
    });
    connect(botonMenu, &QPushButton::clicked, this, &Nivel2::volverAlMenu);
}

