#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QKeyEvent>

class Personaje : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit Personaje(QObject* parent = nullptr);

    QString nombre;
    int vida;
    float velocidad;

    int spriteX;
    int spriteY;
    int spriteAncho;
    int spriteAlto;
    float posX;
    float posY;


    QPixmap hojaSprites;
    QPixmap sprite;

    virtual void recibirDanio(int cantidad);
    virtual void mover();
    virtual void saltar();
    virtual void atacar();
    virtual void esquivar();

protected:
    void keyPressEvent(QKeyEvent* event) override;
};

#endif // PERSONAJE_H
