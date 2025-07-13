#include "nivel2.h"
#include "recursos.h"
#include "goku.h"
#include "taopaipaijefe.h"

#include <QGraphicsPixmapItem>    // Para fondo y barras de vida
#include <QDebug>                 // Para mensajes de depuración
#include <QGraphicsProxyWidget>   // Para botones dentro de la escena
#include <QPushButton>
#include <QTimer>                 // Para control de tiempo de juego y ataques
#include <QMediaPlayer>           // Para musica y sonidos
#include <QAudioOutput>           // Para salida de audio
#include <stdexcept>              // Para manejo de errores con throw


Nivel2::Nivel2(QGraphicsView* vista_, QObject* parent)
    : Nivel(vista_, parent)
{
    escena = nullptr;
    goku = nullptr;
    taoPaiPai = nullptr;
    timerAtaques = nullptr;
    timerActualizacion = nullptr;
    derrotaMostrada = false;
    victoriaMostrada = false;
    terminado = false;
    fondo.load(Recursos::fondoNivel2);
}

void Nivel2::iniciarnivel() {
    try {
        if (!escena)
            escena = new QGraphicsScene();
        else
            escena->clear();
        escena->setSceneRect(0, 0, 800, 600);
        vista->setScene(escena);
        if (!fondo.load(Recursos::fondoNivel2))
            throw std::runtime_error("No se pudo cargar el fondo del nivel 2.");
        QGraphicsPixmapItem* fondoItem = new QGraphicsPixmapItem(fondo.scaled(800, 600));
        escena->addItem(fondoItem);
        if (goku) {
            escena->removeItem(goku);
            goku->disconnect();
            delete goku;
            goku = nullptr;
        }
        if (taoPaiPai) {
            escena->removeItem(taoPaiPai);
            taoPaiPai->disconnect();
            delete taoPaiPai;
            taoPaiPai = nullptr;
        }
        goku = new Goku(vista);
        connect(goku, &Goku::danioRecibido, this, []() {
            qDebug() << "Goku recibió daño";
        });

        taoPaiPai = new TaoPaiPaiJefe(vista);
        taoPaiPai->setObjetivo(goku);

        goku->setPos(100, 400);
        taoPaiPai->setPos(600, 400);

        escena->addItem(goku);
        escena->addItem(taoPaiPai);
        goku->setFocus();

        personajes.clear();
        personajes.append(goku);
        personajes.append(taoPaiPai);

        for (auto* barra : barrasVidaGoku) delete barra;
        barrasVidaGoku.clear();
        for (auto* barra : barrasVidaTao) delete barra;
        barrasVidaTao.clear();

        crearBarrasVida();
        if (!timerActualizacion) {
            timerActualizacion = new QTimer(this);
            connect(timerActualizacion, &QTimer::timeout, this, &Nivel2::actualizar);
        }
        timerActualizacion->start(100);
        if (!timerAtaques) {
            timerAtaques = new QTimer(this);
            connect(timerAtaques, &QTimer::timeout, taoPaiPai, &TaoPaiPaiJefe::lanzarGranada);
        }
        timerAtaques->start(3000);

        musicaNivel2 = new QMediaPlayer(this);
        salidaAudio = new QAudioOutput(this);
        musicaNivel2->setAudioOutput(salidaAudio);
        salidaAudio->setVolume(0.2);
        if (Recursos::sonidoNivel2.isEmpty())
            throw std::runtime_error("Ruta de sonido del nivel 2 vacía.");

        musicaNivel2->setSource(QUrl("qrc" + Recursos::sonidoNivel2));
        musicaNivel2->play();
    }
    catch (const std::exception& e) {
        qDebug() << "Error al iniciar el nivel 2:" << e.what();
    }
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
    for (int i = 0; i < goku->getVida(); ++i) {
        auto* barra = new QGraphicsRectItem(20 + i * 25, 20, 20, 20);
        barra->setBrush(Qt::green);
        escena->addItem(barra);
        barrasVidaGoku.append(barra);
    }
    for (int i = 0; i < taoPaiPai->getVida(); ++i) {
        auto* barra = new QGraphicsRectItem(580 + i * 25, 20, 20, 20);
        barra->setBrush(Qt::red);
        escena->addItem(barra);
        barrasVidaTao.append(barra);
    }
}

void Nivel2::actualizarBarrasVida() {
    for (int i = 0; i < barrasVidaGoku.size(); ++i)
        barrasVidaGoku[i]->setVisible(i < goku->getVida());
    for (int i = 0; i < barrasVidaTao.size(); ++i)
        barrasVidaTao[i]->setVisible(i < taoPaiPai->getVida());
}

void Nivel2::mostrarPantallaGameOver() {
    try {
        if (timerActualizacion) timerActualizacion->stop();
        if (timerAtaques) timerAtaques->stop();
        if (musicaNivel2) musicaNivel2->stop();
        escena->clear();

        QPixmap fondoGameOver(Recursos::fondoGameOverGoku);
        if (fondoGameOver.isNull())
            throw std::runtime_error("No se pudo cargar el fondo de Game Over.");

        auto* fondoItem = new QGraphicsPixmapItem(fondoGameOver.scaled(800, 600));
        fondoItem->setZValue(0);
        escena->addItem(fondoItem);
        fondoItem->setPos(0, 0);

        auto* botonMenu = new QPushButton("Volver al menú");
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
    catch (const std::exception& e) {
        qDebug() << "Error al mostrar Game Over:" << e.what();
    }
}

void Nivel2::mostrarPantallaVictoria() {
    try {
        if (timerActualizacion) timerActualizacion->stop();
        if (timerAtaques) timerAtaques->stop();
        if (musicaNivel2) musicaNivel2->stop();
        escena->clear();

        QPixmap fondoVictoria(Recursos::fondoWinGoku);
        if (fondoVictoria.isNull())
            throw std::runtime_error("No se pudo cargar el fondo de victoria.");

        auto* fondoItem = new QGraphicsPixmapItem(fondoVictoria.scaled(800, 600));
        fondoItem->setZValue(0);
        escena->addItem(fondoItem);
        fondoItem->setPos(0, 0);

        auto* botonMenu = new QPushButton("Volver al menú");
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
    catch (const std::exception& e) {
        qDebug() << "Error al mostrar pantalla de victoria:" << e.what();
    }
}

void Nivel2::volverAlMenu() {
    if (timerActualizacion) timerActualizacion->stop();
    if (timerAtaques) timerAtaques->stop();
    if (musicaNivel2) musicaNivel2->stop();
    detenerSonidosExtras();
    vista->close();
}

void Nivel2::detenerMusica() {
    if (musicaNivel2 && musicaNivel2->isPlaying()) {
        musicaNivel2->stop();
        qDebug() << "Música detenida al cerrar el nivel.";
    }
}

void Nivel2::detenerSonidosExtras() {
    const auto objetos = this->findChildren<QMediaPlayer*>();
    for (QMediaPlayer* s : objetos) {
        if (s->isPlaying()) s->stop();
    }
}

