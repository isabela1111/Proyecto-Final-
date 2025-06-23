#ifndef JUEGO_H
#define JUEGO_H

#include <QObject>
#include "nivel.h"

class Juego : public QObject {
    Q_OBJECT
public:
    explicit Juego(QObject* parent = nullptr);

    Nivel* nivelActual;
    bool enEjecucion;

    void iniciarJuego();
    void cambiarNivel(Nivel* n);
    void actualizar();
    void terminarJuego();
};

#endif // JUEGO_H
