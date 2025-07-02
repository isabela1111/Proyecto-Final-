#include "gokunube.h"
#include "recursos.h"

GokuNube::GokuNube(QGraphicsView* vista, QObject* parent)
    : Personaje(vista, parent), frameActual(0)
{
    velocidad = 10;
    hojaSprites.load(Recursos::gokuNubeSprite);
    setPixmap(hojaSprites.copy(0, 0, 35, 44));

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, &GokuNube::cambiarSprite);
    timerAnimacion->start(200);

    timerMovimiento = new QTimer(this);
    connect(timerMovimiento, &QTimer::timeout, this, &GokuNube::moverAutomatico);
    timerMovimiento->start(50);
}

void GokuNube::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Up) {
        if (y() - velocidad >= 0)
            setY(y() - velocidad);
    }
    else if (event->key() == Qt::Key_Down) {
        if (vista && y() + boundingRect().height() + velocidad <= vista->height()) {
            setY(y() + velocidad);
        }
    }
}

void GokuNube::cambiarSprite() {
    int x = (frameActual % 2) * 35;
    setPixmap(hojaSprites.copy(x, 0, 35, 44));
    frameActual++;
}

void GokuNube::moverAutomatico() {
    setX(x() + 3);
}

GokuNube::~GokuNube() {
    if (timerAnimacion) {
        timerAnimacion->stop();
        delete timerAnimacion;
        timerAnimacion = nullptr;
    }
    if (timerMovimiento) {
        timerMovimiento->stop();
        delete timerMovimiento;
        timerMovimiento = nullptr;
    }
}
