#include "nivel1.h"
#include "recursos.h"
#include "piedra.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsProxyWidget>
#include <QTimer>
#include <QDebug>
#include <QFont>
#include <QRandomGenerator>
#include <QDateTime>
#include <stdexcept>

Nivel1::Nivel1(QGraphicsView* vista_, QObject* parent)
    : Nivel(vista_, parent), textoAltura(nullptr),metrosSubidos(0),vista(vista_),fisicaActiva(false),
    gameOverShown(false),juegoIniciado(false),piedrasPorIntervalo(1)
{
    escena = new QGraphicsScene();
    escena->setSceneRect(0, 0, 800, 600);
    vista->setScene(escena);

    vidasTotales    = 5;
    vidasRestantes  = vidasTotales;

    contadorScroll = 0;
    scrollsDesdeUltimaPiedra = 0;
    scrollsDesdeUltimaPiedra = 0;
    tiempoUltimaPiedra = QDateTime::currentMSecsSinceEpoch();


    temporizador    = nullptr;
    timerCronometro = nullptr;
    timerPiedras    = nullptr;
    musicaNivel1 = nullptr;
    salidaAudio1 = nullptr;

    textoCronometro = new QGraphicsTextItem();
    textoCronometro->setDefaultTextColor(Qt::black);
    textoCronometro->setFont(QFont("Consolas", 30, QFont::Bold));
    textoCronometro->setPos(500, 10);
    textoCronometro->setZValue(2);
    escena->addItem(textoCronometro);
    tiempoRestante = 2 * 60;

    textoAltura = new QGraphicsTextItem("Altura: 0 m");
    textoAltura->setDefaultTextColor(Qt::black);
    textoAltura->setFont(QFont("Consolas", 18, QFont::Bold));
    textoAltura->setPos(30, 530);
    textoAltura->setZValue(2);
    escena->addItem(textoAltura);
}

void Nivel1::iniciarnivel() {
    try {
        fondo.load(Recursos::fondoNivel1);
        if (fondo.isNull()) {
            throw std::runtime_error("No se pudo cargar el fondo del nivel 1.");
        }
        QPixmap fondoEscalado = fondo.scaled(800, ALTURA_SECCION);
        for (int i = 0; i < 2; ++i) {
            QGraphicsPixmapItem* fondoSeccion = new QGraphicsPixmapItem(fondoEscalado);
            fondoSeccion->setZValue(0);
            fondoSeccion->setPos(0, -i * ALTURA_SECCION);
            escena->addItem(fondoSeccion);
            fondosScroll.append(fondoSeccion);
        }
        crearBarrasVida();
        textoCronometro->setPlainText("Tiempo: 02:00");
        escena->addItem(textoCronometro);
        escena->addItem(textoAltura);

        taoPaiPai = new TaoPaiPai(vista, this);
        taoPaiPai->setPos(250, 500);
        taoPaiPai->cayendo = false;
        taoPaiPai->setFocus();
        escena->addItem(taoPaiPai);

        textoInicio = new QGraphicsTextItem("Presiona la letra E para empezar");
        textoInicio->setDefaultTextColor(Qt::black);
        textoInicio->setFont(QFont("Consolas", 20, QFont::Bold));
        textoInicio->setPos(200, 250);
        textoInicio->setZValue(3);
        escena->addItem(textoInicio);
        // SONIDO
        musicaNivel1 = new QMediaPlayer(this);
        salidaAudio1 = new QAudioOutput(this);
        musicaNivel1->setAudioOutput(salidaAudio1);
        salidaAudio1->setVolume(0.25);
        musicaNivel1->setSource(QUrl("qrc" + Recursos::sonidoNivel1));
        musicaNivel1->play();
    }
    catch (const std::exception& e) {
        qDebug() << "Error al iniciar el nivel 1:" << e.what();
    }
}

void Nivel1::comenzarJuego() {
    if (juegoIniciado || gameOverShown) return;
    juegoIniciado = true;
    escena->removeItem(textoInicio);
    taoPaiPai->setPos(250, 500);
    taoPaiPai->cayendo = false;
    taoPaiPai->reiniciarFisica();
    // Cronometro
    timerCronometro = new QTimer(this);
    connect(timerCronometro, &QTimer::timeout, this, &Nivel1::actualizarCronometro);
    timerCronometro->start(1000);
    // Fisica del juego
    QTimer::singleShot(1500, this, &Nivel1::iniciarFisica);
}

void Nivel1::iniciarFisica() {
    fisicaActiva = true;
    taoPaiPai->activarFisica(true);
    temporizador = new QTimer(this);
    connect(temporizador, &QTimer::timeout, this, [=]() {
        if (!gameOverShown) {
            verificarCaida();
            aplicarGravedad();
            verificarColisiones();
        }
    });
    temporizador->start(30);
}

void Nivel1::ajustarDificultad() {
    int etapas = metrosSubidos / 150;
    if (metrosSubidos <= 1000) {
        piedrasPorIntervalo = 1 + etapas;
    }
    else {
        piedrasPorIntervalo = 5;
    }
    int intervalo = 3000 - etapas * 300;
    if (metrosSubidos > 1000) {
        intervalo = qMax(1200, intervalo);
    }
    else {
        intervalo = qMax(700, intervalo);
    }
    if (tiempoRestante < 60) intervalo -= 200;
    if (tiempoRestante < 30) intervalo -= 200;
    intervalo = qMax(700, intervalo);
    if (timerPiedras) {
        timerPiedras->setInterval(intervalo);
    }
}

void Nivel1::verificarCaida() {
    if (!fisicaActiva) return;
    // Verificar subida para mover scroll y crear piedra cada ciertos scrolls
    if (juegoIniciado && taoPaiPai->y() < 100 && !gameOverShown) {
        metrosSubidos += 10;
        textoAltura->setPlainText(QString("Altura: %1 m").arg(metrosSubidos));
        ajustarDificultad();
        for (auto item : escena->items()) {
            if (item != textoAltura && item != textoCronometro && item != textoInicio &&
                !barrasVida.contains(dynamic_cast<QGraphicsRectItem*>(item))) {
                item->moveBy(0, 10);
            }
        }
        contadorScroll++;
        scrollsDesdeUltimaPiedra++;
        for (auto fondo : fondosScroll) {
            if (fondo->y() > 600) {
                fondo->moveBy(0, -2 * ALTURA_SECCION);
            }
        }
        if (metrosSubidos >= 2000 && !gameOverShown) {
            gameOverShown = true;
            QTimer::singleShot(100, this, [this]() {
                mostrarPantallaVictoria();
            });
            return;
        }
    }
    // Verificar generación de piedra por scroll o tiempo
    qint64 tiempoActual = QDateTime::currentMSecsSinceEpoch();
    bool tiempoCumplido = (tiempoActual - tiempoUltimaPiedra >= 5000);
    bool scrollCumplido = (scrollsDesdeUltimaPiedra >= 25);
    if (tiempoCumplido || scrollCumplido) {
        scrollsDesdeUltimaPiedra = 0;
        tiempoUltimaPiedra = tiempoActual;
        if (vista) {
            QPointF esquinaSuperior = vista->mapToScene(0, 0);
            qreal yInicio = esquinaSuperior.y() - 45;
            int xFijo = 305;
            Piedra* piedra = new Piedra(this);
            piedra->setPos(xFijo, yInicio);
            escena->addItem(piedra);
        }
    }
    // Verifica si Tao se cayo abajo del escenario
    if (juegoIniciado && !taoPaiPai->cayendo &&
        taoPaiPai->y() + taoPaiPai->boundingRect().height() > escena->height()) {
        taoPaiPai->cayendo = true;
        quitarVida();
        taoPaiPai->mostrarCaida();
        QTimer::singleShot(500, this, [this]() {
            taoPaiPai->setPos(250, 500);
            taoPaiPai->setFocus();
            taoPaiPai->cayendo = false;
        });
    }
}

void Nivel1::verificarColisiones() {
    QList<QGraphicsItem*> colisiones = taoPaiPai->collidingItems();
    for (QGraphicsItem* item : colisiones) {
        Piedra* piedra = dynamic_cast<Piedra*>(item);
        if (piedra) {
            taoPaiPai->reproducirSonidoGolpe();
            quitarVida();
            escena->removeItem(piedra);
            delete piedra;
            taoPaiPai->cayendo = true;
            taoPaiPai->mostrarCaida();
            QTimer::singleShot(500, this, [this]() {
                taoPaiPai->setPos(250, 500);
                taoPaiPai->cayendo = false;
            });
            break;
        }
    }
}

void Nivel1::aplicarGravedad() {
    if (!gameOverShown && juegoIniciado && !taoPaiPai->cayendo &&
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
    if (tiempoRestante <= 0 && !gameOverShown) {
        gameOverShown = true;
        if (metrosSubidos >= 2000) {
            QTimer::singleShot(100, this, [this]() {
                mostrarPantallaVictoria();
            });
        }
        else {
            QTimer::singleShot(100, this, [this]() {
                mostrarPantallaGameOver();
            });
        }
    }

}

void Nivel1::mostrarPantallaGameOver() {
    try {
        gameOverShown = true;
        if (temporizador) temporizador->stop();
        if (timerCronometro) timerCronometro->stop();
        if (timerPiedras) timerPiedras->stop();
        escena->clear();
        QPixmap fondoGameOver(Recursos::fondoGameOverTao);
        if (fondoGameOver.isNull()) {
            throw std::runtime_error("No se pudo cargar fondoGameOverTao.");
        }
        QGraphicsPixmapItem* fondoItem = new QGraphicsPixmapItem(fondoGameOver.scaled(800, 600));
        escena->addItem(fondoItem);

        QPushButton* botonMenu = new QPushButton("Volver al menú");
        botonMenu->setFixedSize(200, 50);
        botonMenu->setStyleSheet("background-color: white; color: black; font-weight: bold; border-radius: 10px;");
        QGraphicsProxyWidget* proxy = escena->addWidget(botonMenu);
        proxy->setZValue(101);

        QPointF centroVista = vista->mapToScene(vista->viewport()->rect().center());
        proxy->setPos(centroVista.x() - 100, centroVista.y() + 150);
        connect(botonMenu, &QPushButton::clicked, [this]() {
            emit volverAlMenu();
            vista->close();
        });
    }
    catch (const std::exception& e) {
        qDebug() << "Error al mostrar pantalla Game Over:" << e.what();
    }
}

void Nivel1::mostrarPantallaVictoria() {
    try {
        gameOverShown = true;
        escena->clear();
        QPixmap fondoWin(Recursos::fondoWinTao);
        if (fondoWin.isNull()) {
            throw std::runtime_error("No se pudo cargar fondoWinTao.");
        }
        QGraphicsPixmapItem* fondoItem = new QGraphicsPixmapItem(fondoWin.scaled(800, 600));
        escena->addItem(fondoItem);

        QPushButton* botonMenu = new QPushButton("Volver al menú");
        botonMenu->setFixedSize(200, 50);
        botonMenu->setStyleSheet("background-color: white; color: black; font-weight: bold; border-radius: 10px;");
        QGraphicsProxyWidget* proxy = escena->addWidget(botonMenu);
        proxy->setZValue(101);

        QPointF centroVista = vista->mapToScene(vista->viewport()->rect().center());
        proxy->setPos(centroVista.x() - 100, centroVista.y() + 150);
        connect(botonMenu, &QPushButton::clicked, [this]() {
            emit volverAlMenu();
            vista->close();
        });
    }
    catch (const std::exception& e) {
        qDebug() << "Error al mostrar pantalla de victoria:" << e.what();
    }
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

void Nivel1::quitarVida() {
    if (vidasRestantes > 0) {
        vidasRestantes--;
        if (taoPaiPai) {
            taoPaiPai->reproducirSonidoGolpe();
        }
        if (vidasRestantes >= 0 && vidasRestantes < barrasVida.size()) {
            QGraphicsRectItem* barra = barrasVida.takeAt(vidasRestantes);
            if (barra) {
                escena->removeItem(barra);
                delete barra;
            }
        }
        if (vidasRestantes <= 0 && !gameOverShown) {
            QTimer::singleShot(200, this, [this]() {
                mostrarPantallaGameOver();
            });
        }
    }
}

void Nivel1::detenerMusica() {
    if (musicaNivel1 && musicaNivel1->isPlaying()) {
        musicaNivel1->stop();
        qDebug() << "Musica del nivel 1 detenida.";
    }
}

