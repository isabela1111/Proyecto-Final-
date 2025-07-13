#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsProxyWidget>
#include <QPushButton>
#include <QImage>
#include <QFont>
#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <stdexcept>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->botonSalir, &QPushButton::clicked, this, &MainWindow::cerrarAplicacion);
    connect(ui->Reglas, &QPushButton::clicked, this, &MainWindow::mostrarReglas);
    this->showMaximized();
    // Audio
    try {
        reproductorIntro = new QMediaPlayer(this);
        salidaAudio = new QAudioOutput(this);
        reproductorIntro->setAudioOutput(salidaAudio);
        reproductorIntro->setSource(QUrl("qrc" + Recursos::sonidoIntro));
        salidaAudio->setVolume(0.5);
        reproductorIntro->play();
    }
    catch (const std::exception& e) {
        qDebug() << "Error al cargar o reproducir la música de inicio:" << e.what();
    }
    // Escena
    escena = new QGraphicsScene(this);
    if (!ui->graphicsView_)
        throw std::runtime_error("Error: graphicsView_ no inicializado en la interfaz.");
    ui->graphicsView_->setScene(escena);
    ui->graphicsView_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView_->setFrameStyle(QFrame::NoFrame);

    QImage fondoOriginal(Recursos::fondoMenu);
    if (fondoOriginal.isNull())
        throw std::runtime_error("Error: no se pudo cargar el fondo del menú.");
    QImage fondoEscalado = fondoOriginal.scaled(
        ui->graphicsView_->width(),
        ui->graphicsView_->height(),
        Qt::IgnoreAspectRatio,
        Qt::SmoothTransformation
        );
    escena->setSceneRect(0, 0, ui->graphicsView_->width(), ui->graphicsView_->height());
    ui->graphicsView_->setBackgroundBrush(QBrush(fondoEscalado));
    // Titulo
    QString textoTitulo = "Dragon Ball";
    QFont fuenteTitulo("Impact", 80, QFont::Bold);
    titulo = escena->addText(textoTitulo);
    titulo->setFont(fuenteTitulo);
    titulo->setDefaultTextColor(QColor("#0a2c6b"));

    QRectF rectTitulo = titulo->boundingRect();
    float xCentro = (escena->width() - rectTitulo.width()) / 2;
    titulo->setPos(xCentro, 18);
    // Botones
    int botonAncho = 300;
    int botonAlto = 60;
    int espacio = 60;
    int xBoton = (escena->width() - botonAncho) / 2;
    int yInicio = 300;

    botonNivel1 = new QPushButton("Nivel 1: Torre Karin");
    botonNivel1->setFixedSize(botonAncho, botonAlto);
    botonNivel1->setStyleSheet("background-color: black; color: white; border-radius: 10px;");
    escena->addWidget(botonNivel1)->setPos(xBoton, yInicio);
    connect(botonNivel1, &QPushButton::clicked, this, &MainWindow::iniciarNivel1);

    botonNivel2 = new QPushButton("Nivel 2: Goku vs Tao Pai Pai");
    botonNivel2->setFixedSize(botonAncho, botonAlto);
    botonNivel2->setStyleSheet("background-color: black; color: white; border-radius: 10px;");
    escena->addWidget(botonNivel2)->setPos(xBoton, yInicio + botonAlto + espacio);
    connect(botonNivel2, &QPushButton::clicked, this, &MainWindow::iniciarNivel2);

    botonNivel3 = new QPushButton("Nivel 3: Vuelo hacia la base");
    botonNivel3->setFixedSize(botonAncho, botonAlto);
    botonNivel3->setStyleSheet("background-color: black; color: white; border-radius: 10px;");
    escena->addWidget(botonNivel3)->setPos(xBoton, yInicio + 2 * (botonAlto + espacio));
    connect(botonNivel3, &QPushButton::clicked, this, &MainWindow::iniciarNivel3);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::iniciarNivel1() {
    if (reproductorIntro && reproductorIntro->isPlaying())
        reproductorIntro->stop();

    QGraphicsView* vista = new QGraphicsView();
    vista->setAttribute(Qt::WA_DeleteOnClose);
    vista->setFixedSize(800, 600);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setFrameStyle(QFrame::NoFrame);
    vista->setFocus();

    nivel1 = new Nivel1(vista, this);
    nivel1->iniciarnivel();

    vista->setScene(nivel1->escena);
    vista->show();

    connect(vista, &QObject::destroyed, this, [this]() {
        if (nivel1) {
            nivel1->detenerMusica();
            nivel1 = nullptr;
        }
    });
}

void MainWindow::iniciarNivel2() {
    if (reproductorIntro && reproductorIntro->isPlaying())
        reproductorIntro->stop();

    QGraphicsView* vista = new QGraphicsView();
    vista->setAttribute(Qt::WA_DeleteOnClose);
    vista->setFixedSize(800, 600);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setFrameStyle(QFrame::NoFrame);
    vista->setFocus();

    nivel2 = new Nivel2(vista, this);
    nivel2->iniciarnivel();

    vista->setScene(nivel2->escena);
    vista->show();
    connect(vista, &QObject::destroyed, this, [this]() {
        if (nivel2) {
            nivel2->detenerMusica();
            nivel2 = nullptr;
        }
    });
}


void MainWindow::iniciarNivel3() {
    if (reproductorIntro && reproductorIntro->isPlaying())
        reproductorIntro->stop();
    QGraphicsView* vista = new QGraphicsView();
    vista->setAttribute(Qt::WA_DeleteOnClose);
    vista->setFixedSize(800, 600);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setFrameStyle(QFrame::NoFrame);
    vista->setFocus();

    nivel3 = new Nivel3(vista);
    nivel3->iniciarnivel();

    vista->setScene(nivel3->escena);
    vista->show();

    connect(vista, &QObject::destroyed, this, [this]() {
        if (nivel3) {
            nivel3->detenerMusica();
            nivel3 = nullptr;
        }
    });
}

void MainWindow::cerrarAplicacion() {
    QCoreApplication::quit();
}

void MainWindow::mostrarReglas() {
    QDialog* ventanaReglas = new QDialog(this);
    ventanaReglas->setWindowTitle("Reglas del Juego");
    ventanaReglas->setFixedSize(700, 500);

    QLabel* imagen = new QLabel(ventanaReglas);
    imagen->setPixmap(QPixmap(Recursos::Reglas).scaled(680, 480, Qt::KeepAspectRatio));
    imagen->setAlignment(Qt::AlignCenter);

    QVBoxLayout* layout = new QVBoxLayout(ventanaReglas);
    layout->addWidget(imagen);

    ventanaReglas->setLayout(layout);
    ventanaReglas->exec();
    if (ui->graphicsView_) {
        ui->graphicsView_->setFocusPolicy(Qt::StrongFocus);
        ui->graphicsView_->setFocus();
    }
}
