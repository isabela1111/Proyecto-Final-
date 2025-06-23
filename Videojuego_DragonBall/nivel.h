#ifndef NIVEL_H
#define NIVEL_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QList>
#include "personaje.h"

class Nivel : public QObject {
    Q_OBJECT
public:
    explicit Nivel(QObject* parent = nullptr);

    QPixmap fondo;
    bool terminado;
    QList<Personaje*> personajes;
    QGraphicsScene* escena;

    virtual void iniciarnivel();
    virtual void actualizar();
    virtual void terminar();
    virtual void mostrarCinematicaFinal();
};

#endif // NIVEL_H
