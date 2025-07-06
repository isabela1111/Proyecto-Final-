#include "goku.h"
#include <QKeyEvent>
#include <QDebug>
#include <QTimer>
#include <QSoundEffect>
#include "recursos.h"

Goku::Goku(QGraphicsView* vista, QObject* parent)
    : Personaje(vista, parent),
      frameActual(0),
      tiempoSalto(0),
      enElAire(false),
      modoAtaque(false)
{
    nombre = "Goku";
    vida = 5;
    velocidad = 7;
    spriteAncho = 120;  // Nuevo tamaño
    spriteAlto = 120;

    hojaSprites.load(Recursos::gokuSprite);
    setPixmap(hojaSprites.copy(0, 0, spriteAncho, spriteAlto));
    setPos(100, 400);

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    temporizadorAtaque = new QTimer(this);
    temporizadorAtaque->setSingleShot(true);
    connect(temporizadorAtaque, &QTimer::timeout, this, [this]() {
        modoAtaque = false;
    });
}

void Goku::mover() {
    frameActual = (frameActual + 1) % 8; // cuadros 1 a 8 en fila 1
    int x = (frameActual + 1) * spriteAncho;  // del segundo al noveno cuadro
    int y = 0; // Fila 1 → y = 0
    setPixmap(hojaSprites.copy(x, y, spriteAncho, spriteAlto));
}

void Goku::saltar() {
    if (!enElAire) {
        tiempoSalto = 15;
        enElAire = true;

        QSoundEffect* sonidoSalto = new QSoundEffect(this);
        sonidoSalto->setSource(QUrl("qrc:/Recursos/Sonidos/salto.wav"));
        sonidoSalto->play();
    }
}

void Goku::atacar() {
    qDebug() << "Goku realiza un ataque.";
    int x = 0;
    int y = 8 * spriteAlto;  // Fila 9 → y = 8 * 120 = 960
    setPixmap(hojaSprites.copy(x, y, spriteAncho, spriteAlto));
    modoAtaque = true;
    temporizadorAtaque->start(300);

    QSoundEffect* sonidoGolpe = new QSoundEffect(this);
    sonidoGolpe->setSource(QUrl("qrc:/Recursos/Sonidos/golpe.wav"));
    sonidoGolpe->play();
}

void Goku::devolverGranada() {
    modoAtaque = true;
    temporizadorAtaque->start(300);
    qDebug() << "Goku está intentando devolver la granada.";
}

bool Goku::estaEnModoAtaque() const {
    return modoAtaque;
}

void Goku::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
    case Qt::Key_A:
        if (x() > 0) moveBy(-velocidad, 0);
        mover();
        break;
    case Qt::Key_D:
        if (x() + boundingRect().width() < limites.width()) moveBy(velocidad, 0);
        mover();
        break;
    case Qt::Key_Space:
        atacar();
        break;
    case Qt::Key_W:
        saltar();
        break;
    default:
        break;
    }

    // Simulación simple de salto
    if (tiempoSalto > 0) {
        moveBy(0, -10);
        tiempoSalto--;
    } else if (enElAire) {
        moveBy(0, 10); // gravedad
        if (y() >= 400) {
            enElAire = false;
            setY(400);
        }
    }
}
