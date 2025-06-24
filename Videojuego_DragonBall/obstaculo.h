#ifndef OBSTACULO_H
#define OBSTACULO_H

#include <QGraphicsPixmapItem>

class Obstaculo : public QGraphicsPixmapItem {
public:
    Obstaculo(QPixmap sprite, int x, int y);
};

#endif // OBSTACULO_H
