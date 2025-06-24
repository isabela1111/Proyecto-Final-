#include "nivel1.h"
#include "recursos.h"
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QDebug>

Nivel1::Nivel1(QGraphicsView* vista, QObject* parent)
    : Nivel(parent), vista(vista)
{
    escena = new QGraphicsScene();
    escena->setSceneRect(0, 0, 800, 600);

    alturaActual = 0;
    alturaMeta = 600;
}

void Nivel1::iniciarnivel() {
    fondo.load(Recursos::fondoNivel1);

    if (fondo.isNull()) {
        qDebug() << "No se pudo cargar el fondo del nivel 1.";
        return;
    }

    QPixmap fondoEscalado = fondo.scaled(800, 600, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    escena->addPixmap(fondoEscalado);

    taoPaiPai = new TaoPaiPai(vista, this);
    escena->addItem(taoPaiPai);
}

void Nivel1::generarObstaculos() {
    // Aquí irá la lógica para agregar obstáculos
}

void Nivel1::verificarCaida() {
    if (taoPaiPai->y() > escena->height()) {
        qDebug() << "Tao Pai Pai ha caído.";
    }
}

void Nivel1::aplicarGravedad() {
    if (taoPaiPai->y() + taoPaiPai->boundingRect().height() < escena->height()) {
        taoPaiPai->moveBy(0, 5);
    }
}

