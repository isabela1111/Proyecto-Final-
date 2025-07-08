#include "mainwindow.h"

#include "ui_mainwindow.h"
#include <QGraphicsProxyWidget>
#include <QPushButton>
#include <QImage>
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showMaximized();

    reproductorIntro = new QMediaPlayer(this);
    salidaAudio = new QAudioOutput(this);
    reproductorIntro->setAudioOutput(salidaAudio);
    reproductorIntro->setSource(QUrl("qrc" + Recursos::sonidoIntro));
    salidaAudio->setVolume(0.5);
    reproductorIntro->play();

    escena = new QGraphicsScene(this);
    ui->graphicsView_->setScene(escena);
    ui->graphicsView_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView_->setFrameStyle(QFrame::NoFrame);

    QImage fondoOriginal(Recursos::fondoMenu);
    QImage fondoEscalado = fondoOriginal.scaled(
        ui->graphicsView_->width(),
        ui->graphicsView_->height(),
        Qt::IgnoreAspectRatio,
        Qt::SmoothTransformation
        );

    escena->setSceneRect(0, 0, ui->graphicsView_->width(), ui->graphicsView_->height());
    ui->graphicsView_->setBackgroundBrush(QBrush(fondoEscalado));
    // Titulo y botones
    QString textoTitulo = "Dragon Ball";
    QFont fuenteTitulo("Impact", 80, QFont::Bold);
    titulo = escena->addText(textoTitulo);
    titulo->setFont(fuenteTitulo);
    titulo->setDefaultTextColor(QColor("#0a2c6b"));

    QRectF rectTitulo = titulo->boundingRect();
    float xCentro = (escena->width() - rectTitulo.width()) / 2;
    titulo->setPos(xCentro, 18);

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
    nivel1 = new Nivel1(vista, this);
    nivel1->iniciarnivel();

    vista->setScene(nivel1->escena);
    vista->setFixedSize(800, 600);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setFrameStyle(QFrame::NoFrame);
    vista->setFocus();
    vista->show();
}


void MainWindow::iniciarNivel2() {
    if (reproductorIntro && reproductorIntro->isPlaying())
        reproductorIntro->stop();

    QGraphicsView* vista = new QGraphicsView();
    nivel2 = new Nivel2(vista, this);
    nivel2->iniciarnivel();

    vista->setScene(nivel2->escena);
    vista->setFixedSize(800, 600);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setFrameStyle(QFrame::NoFrame);
    vista->setFocus();
    vista->show();
}


void MainWindow::iniciarNivel3() {
    if (reproductorIntro && reproductorIntro->isPlaying())
        reproductorIntro->stop();

    QGraphicsView* vista = new QGraphicsView();
    nivel3 = new Nivel3(vista);
    nivel3->iniciarnivel();

    vista->setScene(nivel3->escena);
    vista->setFixedSize(800, 600);
    vista->show();
}
