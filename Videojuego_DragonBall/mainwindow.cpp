#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "nivel1.h"
#include "nivel2.h"
#include "nivel3.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(800, 700);

    escena = new QGraphicsScene(this);
    ui->graphicsView->setScene(escena);

    QImage fondoMenu(":/Recursos/FondoMenu.png");
    escena->setSceneRect(0, 0, fondoMenu.width(), fondoMenu.height());
    ui->graphicsView->setBackgroundBrush(QBrush(fondoMenu));

    titulo = escena->addText("Dragon Ball");      // Título del juego
    titulo->setDefaultTextColor(Qt::white);
    titulo->setFont(QFont("Arial", 18, QFont::Bold));
    titulo->setPos(180, 30);


    botonNivel1 = new QPushButton("Nivel 1: Torre Karin");
    botonNivel1->setFixedSize(300, 40);
    escena->addWidget(botonNivel1)->setPos(250, 100);
    connect(botonNivel1, &QPushButton::clicked, this, &MainWindow::iniciarNivel1);

    descripcion1 = escena->addText("Simulacion de gravedad y obstaculos que\nralentizan el ascenso. Mantente en movimiento.");
    descripcion1->setDefaultTextColor(Qt::white);
    descripcion1->setFont(QFont("Arial", 10));
    descripcion1->setPos(200, 150);

    botonNivel2 = new QPushButton("Nivel 2: Goku vs Tao Pai Pai");
    botonNivel2->setFixedSize(300, 40);
    escena->addWidget(botonNivel2)->setPos(250, 220);
    connect(botonNivel2, &QPushButton::clicked, this, &MainWindow::iniciarNivel2);

    descripcion2 = escena->addText("Combate tactico.");
    descripcion2->setDefaultTextColor(Qt::white);
    descripcion2->setFont(QFont("Arial", 10));
    descripcion2->setPos(200, 270);


    botonNivel3 = new QPushButton("Nivel 3: Vuelo hacia la base");
    botonNivel3->setFixedSize(300, 40);
    escena->addWidget(botonNivel3)->setPos(250, 340);
    connect(botonNivel3, &QPushButton::clicked, this, &MainWindow::iniciarNivel3);

    descripcion3 = escena->addText("Esquiva aviones\nenemigos. ");
    descripcion3->setDefaultTextColor(Qt::white);
    descripcion3->setFont(QFont("Arial", 10));
    descripcion3->setPos(200, 390);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::iniciarNivel1() {
    escena->clear();
    Nivel1* nivel = new Nivel1(this);
}

void MainWindow::iniciarNivel2() {
    escena->clear();
    Nivel2* nivel = new Nivel2(this);
}

void MainWindow::iniciarNivel3() {
    escena->clear();
    Nivel3* nivel = new Nivel3(this);
}
