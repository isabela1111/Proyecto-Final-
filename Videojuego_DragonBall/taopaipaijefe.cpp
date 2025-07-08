#include "taopaipaijefe.h"
#include "granada.h"
#include "recursos.h"
#include <QGraphicsScene>
#include <QDebug>
#include <QTimer>
#include <QRandomGenerator>
#include <QSoundEffect>

TaoPaiPaiJefe::TaoPaiPaiJefe(QGraphicsView* vista, QObject* parent)
    : Personaje(vista, parent), objetivoJugador(nullptr), direccion(-1),
    frameMovimiento(0), frameAtaque(0), estaCayendo(false), estaVisible(false)
{
    nombre = "Tao Pai Pai";
    vida = 5;
    velocidad = 2.5;
    setScale(2.8);

    setPixmap(QPixmap(Recursos::TaoAparicionDe));
    setVisible(false);
    setFlag(QGraphicsItem::ItemIsFocusable, false);
    // Aparicion
    posicionesAparicion << QPointF(100, 380) << QPointF(400, 380) << QPointF(650, 380);
    // Temporizadores
    aparicionTimer = new QTimer(this);
    connect(aparicionTimer, &QTimer::timeout, this, &TaoPaiPaiJefe::aparecer);
    aparicionTimer->start(3000);

    desaparicionTimer = new QTimer(this);
    connect(desaparicionTimer, &QTimer::timeout, this, &TaoPaiPaiJefe::desaparecer);

    ataqueTimer = new QTimer(this);
    connect(ataqueTimer, &QTimer::timeout, this, &TaoPaiPaiJefe::atacar);
    ataqueTimer->start(700);

    QTimer* granadaTimer = new QTimer(this);
    connect(granadaTimer, &QTimer::timeout, this, &TaoPaiPaiJefe::lanzarGranada);
    granadaTimer->start(5000); // Lanza cada 5s
}

void TaoPaiPaiJefe::aparecer() {
    if (!scene()) return;
    estaVisible = true;
    int idx = QRandomGenerator::global()->bounded(posicionesAparicion.size());
    setPos(posicionesAparicion[idx]);
    bool haciaIzquierda = objetivoJugador && x() > objetivoJugador->x();
    hojaMovimiento.load(haciaIzquierda ? Recursos::TaoAparicionIz : Recursos::TaoAparicionDe);

    spriteAncho = 40;
    spriteAlto = 57;
    for (int i = 0; i < 4; ++i) {
        QTimer::singleShot(i * 100, this, [this, i]() {
            setPixmap(hojaMovimiento.copy(i * spriteAncho, 0, spriteAncho, spriteAlto));
        });
    }
    setVisible(true);
    desaparicionTimer->start(1500); // Se borra tras 1.5 segundos
}

void TaoPaiPaiJefe::desaparecer() {
    estaVisible = false;
    setVisible(false);
}

void TaoPaiPaiJefe::atacar() {
    if (!estaVisible || !objetivoJugador || estaCayendo) return;
    if (qAbs(x() - objetivoJugador->x()) < 100) {
        bool haciaIzquierda = x() > objetivoJugador->x();
        hojaMovimiento.load(haciaIzquierda ? Recursos::TaoAtaqueA : Recursos::TaoAtaqueD);

        spriteAncho = 265 / 7;
        spriteAlto = 67;

        setPixmap(hojaMovimiento.copy(frameAtaque * spriteAncho, 0, spriteAncho, spriteAlto));
        frameAtaque = (frameAtaque + 1) % 7;
        if (collidesWithItem(objetivoJugador)) {
            objetivoJugador->recibirDanio(1);
        }
    }
}

void TaoPaiPaiJefe::lanzarGranada() {
    if (!estaVisible || !objetivoJugador || !scene()) return;

    hojaMovimiento.load(Recursos::TaoGranada);
    spriteAncho = 224 / 5;
    spriteAlto = 63;
    for (int i = 0; i < 5; ++i) {
        QTimer::singleShot(i * 100, this, [this, i]() {
            setPixmap(hojaMovimiento.copy(i * spriteAncho, 0, spriteAncho, spriteAlto));
        });
    }
    QTimer::singleShot(300, this, [this]() {
        if (!scene()) return;
        // Ajuste de salida para que aparezca desde la mano de Tao
        qreal xSalida = x() + 30 * scale();
        qreal ySalida = y() + 15 * scale();

        Granada* granada = new Granada(xSalida, ySalida, objetivoJugador, false);
        granada->setZValue(1);
        scene()->addItem(granada);
        qDebug() << "Tao lanza granada desde (" << xSalida << "," << ySalida << ")";
    });
}

void TaoPaiPaiJefe::recibirDanio(int cantidad) {
    if (!estaVisible || estaCayendo) return;
    vida -= cantidad;
    qDebug() << nombre << "recibió" << cantidad << "de daño. Vida restante:" << vida;
    // Reproduce sonido de daño
    QSoundEffect* sonidoTaoDanio = new QSoundEffect(this);
    sonidoTaoDanio->setSource(QUrl("qrc:/Recursos/Sonidos/taodanio.wav"));
    sonidoTaoDanio->play();
    if (vida <= 0) {
        setVisible(false);
        estaVisible = false;
        return;
    }
    hojaMovimiento.load(Recursos::TaoCaidoH);
    spriteAncho = 60;
    spriteAlto = 31;
    setPixmap(hojaMovimiento.copy(0, 0, spriteAncho, spriteAlto));
    estaCayendo = true;
    QTimer::singleShot(600, this, [this]() {
        estaCayendo = false;
    });
}


void TaoPaiPaiJefe::setObjetivo(Personaje* jugador) {
    objetivoJugador = jugador;
}
