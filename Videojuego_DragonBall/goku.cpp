#include "goku.h"
#include <QKeyEvent>
#include <QDebug>
#include <QTimer>
#include <QSoundEffect>
#include "recursos.h"

Goku::Goku(QGraphicsView* vista, QObject* parent)
    : Personaje(vista, parent),
    frameActual(0),tiempoSalto(0),enElAire(false),modoAtaque(false),ataqueAlterno(false)
{
    nombre = "Goku";
    vida = 5;
    velocidad = 7;

    setPos(100, 400);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    temporizadorAtaque = new QTimer(this);
    temporizadorAtaque->setSingleShot(true);
    connect(temporizadorAtaque, &QTimer::timeout, this, [this]() {
        modoAtaque = false;
    });

    temporizadorSalto = new QTimer(this);
    connect(temporizadorSalto, &QTimer::timeout, this, &Goku::actualizarSalto);
    temporizadorSalto->start(30);

    hojaSprites.load(Recursos::gokuRunD);
    spriteAncho = 127;
    spriteAlto = 146;
    frameActual = 0;
    setPixmap(hojaSprites.copy(0, 0, spriteAncho, spriteAlto));
}


void Goku::moverIzquierda() {
    hojaSprites.load(Recursos::gokuRunA);
    spriteAncho = 127;
    spriteAlto = 146;

    frameActual = (frameActual + 1) % 7;
    setPixmap(hojaSprites.copy(frameActual * spriteAncho, 0, spriteAncho, spriteAlto));
    moveBy(-velocidad, 0);
}

void Goku::moverDerecha() {
    hojaSprites.load(Recursos::gokuRunD);
    spriteAncho = 127;
    spriteAlto = 146;

    frameActual = (frameActual + 1) % 7;
    setPixmap(hojaSprites.copy(frameActual * spriteAncho, 0, spriteAncho, spriteAlto));
    moveBy(velocidad, 0);
}

void Goku::saltar() {
    if (!enElAire) {
        hojaSprites.load(Recursos::gokuJump);
        spriteAncho = 126;
        spriteAlto = 107;

        frameActual = 0;
        enElAire = true;
        tiempoSalto = 15;

        QSoundEffect* sonidoSalto = new QSoundEffect(this);
        sonidoSalto->setSource(QUrl("qrc:/Recursos/Sonidos/salto.wav"));
        sonidoSalto->play();
    }
}

void Goku::actualizarSalto() {
    if (enElAire) {
        frameActual = (frameActual + 1) % 7;
        setPixmap(hojaSprites.copy(frameActual * spriteAncho, 0, spriteAncho, spriteAlto));

        if (tiempoSalto > 0) {
            moveBy(0, -10);
            tiempoSalto--;
        } else {
            moveBy(0, 10);
            if (y() >= 400) {
                setY(400);
                enElAire = false;
            }
        }
    }
}

void Goku::atacar() {
    modoAtaque = true;

    QSoundEffect* sonidoGolpe = new QSoundEffect(this);
    sonidoGolpe->setSource(QUrl("qrc:/Recursos/Sonidos/golpe.wav"));
    sonidoGolpe->play();

    if (!ataqueAlterno) {
        hojaSprites.load(Recursos::gokuAtaque1);
        spriteAncho = 127;
        spriteAlto = 114;
        frameActual = (frameActual + 1) % 6;
    } else {
        hojaSprites.load(Recursos::gokuAtaque2);
        spriteAncho = 131;
        spriteAlto = 136;
        frameActual = (frameActual + 1) % 4;
    }
    setPixmap(hojaSprites.copy(frameActual * spriteAncho, 0, spriteAncho, spriteAlto));
    ataqueAlterno = !ataqueAlterno;

    QList<QGraphicsItem*> colisiones = collidingItems();
    for (QGraphicsItem* item : colisiones) {
        if (Personaje* enemigo = dynamic_cast<Personaje*>(item)) {
            if (enemigo != this) {
                enemigo->recibirDanio(1);
            }
        }
    }
    temporizadorAtaque->start(300);
}

void Goku::mostrarCaida() {
    hojaSprites.load(Recursos::gokuCaido);
    spriteAncho = 129;
    spriteAlto = 78;
    setPixmap(hojaSprites.copy(0, 0, spriteAncho, spriteAlto));
}


void Goku::recibirDanio(int cantidad) {
    vida -= cantidad;
    qDebug() << nombre << "recibió" << cantidad << "de daño. Vida restante:" << vida;

    QSoundEffect* sonidoGokuDanio = new QSoundEffect(this);
    sonidoGokuDanio->setSource(QUrl("qrc:/Recursos/Sonidos/gokudanio.wav"));
    sonidoGokuDanio->play();
    mostrarCaida();
    emit danioRecibido();
}

bool Goku::estaEnModoAtaque() const {
    return modoAtaque;
}

void Goku::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
    case Qt::Key_A:
        if (x() > 0) moverIzquierda();
        break;
    case Qt::Key_D:
        if (x() + boundingRect().width() < limites.width()) moverDerecha();
        break;
    case Qt::Key_S:
        atacar();
        break;
    case Qt::Key_W:
        saltar();
        break;
    default:
        break;
    }
}

