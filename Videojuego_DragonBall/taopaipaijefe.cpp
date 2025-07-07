#include "taopaipaijefe.h"
#include "granada.h"
#include "recursos.h"
#include <QGraphicsScene>
#include <QDebug>

TaoPaiPaiJefe::TaoPaiPaiJefe(QGraphicsView* vista, QObject* parent)
    : Personaje(vista, parent), objetivoJugador(nullptr), direccion(-1),
    frameMovimiento(0), frameAtaque(0), estaCayendo(false)
{
    nombre = "Tao Pai Pai";
    vida = 5;
    velocidad = 2.5;

    hojaMovimiento.load(Recursos::TaoRunA);
    spriteAlto = 59;
    setPixmap(hojaMovimiento.copy(0, 0, spriteAncho, spriteAlto));
    setScale(2.0);
    setPos(600, 400 - spriteAlto * scale());

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    movimientoTimer = new QTimer(this);
    connect(movimientoTimer, &QTimer::timeout, this, &TaoPaiPaiJefe::mover);
    movimientoTimer->start(80);

    ataqueTimer = new QTimer(this);
    connect(ataqueTimer, &QTimer::timeout, this, &TaoPaiPaiJefe::atacar);
    ataqueTimer->start(700);
}

void TaoPaiPaiJefe::mover() {
    if (!scene() || estaCayendo) return;
    qreal nuevaX = x() + direccion * velocidad;
    qreal anchoEscena = scene()->width();
    // Verifica si esta en los extremos
    bool enExtremoIzquierdo = nuevaX <= 0;
    bool enExtremoDerecho = nuevaX + boundingRect().width() * scale() >= anchoEscena;
    if (enExtremoIzquierdo || enExtremoDerecho) {
        direccion *= -1;
        lanzarGranada();
    } else {
        moveBy(direccion * velocidad, 0);
    }
    hojaMovimiento.load(direccion == -1 ? Recursos::TaoRunA : Recursos::TaoRunD);
    spriteAncho = 54;
    spriteAlto = 59;
    frameMovimiento = (frameMovimiento + 1) % 5;
    setPixmap(hojaMovimiento.copy(frameMovimiento * spriteAncho, 0, spriteAncho, spriteAlto));
    setScale(2.0);
}

void TaoPaiPaiJefe::atacar() {
    if (!objetivoJugador || estaCayendo) return;
    if (qAbs(x() - objetivoJugador->x()) < 100) {
        bool haciaIzquierda = x() > objetivoJugador->x();
        hojaMovimiento.load(haciaIzquierda ? Recursos::TaoAtaqueA : Recursos::TaoAtaqueD);
        spriteAncho = 554 / 10;
        spriteAlto = 67;
        setPixmap(hojaMovimiento.copy(frameAtaque * spriteAncho, 0, spriteAncho, spriteAlto));
        setScale(2.0);
        frameAtaque = (frameAtaque + 1) % 10;
        if (collidesWithItem(objetivoJugador)) {
            objetivoJugador->recibirDanio(1);
        }
    }
}

void TaoPaiPaiJefe::lanzarGranada() {
    if (!objetivoJugador || !scene()) return;
    qreal xSalida = x() + (boundingRect().width() * scale()) / 2 - 10;
    qreal ySalida = y() + (boundingRect().height() * scale()) / 2;
    Granada* granada = new Granada(xSalida, ySalida, objetivoJugador, false);
    granada->setZValue(1);
    scene()->addItem(granada);
    qDebug() << "Tao lanza granada desde (" << xSalida << "," << ySalida << ")";
}

void TaoPaiPaiJefe::setObjetivo(Personaje* jugador) {
    objetivoJugador = jugador;

}

void TaoPaiPaiJefe::recibirDanio(int cantidad) {
    if (estaCayendo) return;
    vida -= cantidad;
    qDebug() << nombre << "recibió" << cantidad << "de daño. Vida restante:" << vida;

    if (vida <= 0) {
        setVisible(false);
        return;
    }
    hojaMovimiento.load(Recursos::TaoCaidoH);
    spriteAncho = 64;
    spriteAlto = 34;
    setPixmap(hojaMovimiento.copy(0, 0, spriteAncho, spriteAlto));
    estaCayendo = true;
    QTimer::singleShot(600, this, [this]() {
        estaCayendo = false;
    });
}
