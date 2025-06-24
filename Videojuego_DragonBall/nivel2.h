#ifndef NIVEL2_H
#define NIVEL2_H

#include "nivel.h"
#include "goku.h"
#include "taopaipai.h"

class Nivel2 : public Nivel {
    Q_OBJECT
public:
    explicit Nivel2(QObject* parent = nullptr);

    Goku* goku;
    TaoPaiPai* taoPaiPai;

    void gestionarAtaques();
};

#endif // NIVEL2_H
