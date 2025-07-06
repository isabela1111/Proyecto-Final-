#ifndef NIVEL_H
#define NIVEL_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QList>
#include <QPushButton>
#include "personaje.h"

class Nivel : public QObject {
    Q_OBJECT
public:
    explicit Nivel(QObject* parent = nullptr);

    QPixmap fondo;
    bool terminado = false;
    QList<Personaje*> personajes;
    QGraphicsScene* escena = nullptr;
    QGraphicsView* vista = nullptr;

    virtual void iniciarnivel();
    virtual void actualizar();
    virtual void terminar();
    virtual void mostrarCinematicaFinal();

protected:
    void mostrarPantallaFinal(const QPixmap& fondo, bool victoria);

signals:
    void volverAlMenu();
    void nivelTerminado();

};

#endif // NIVEL_H
