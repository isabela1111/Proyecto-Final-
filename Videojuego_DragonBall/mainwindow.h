#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QGraphicsTextItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void iniciarNivel1();
    void iniciarNivel2();
    void iniciarNivel3();

private:
    Ui::MainWindow *ui;
    QGraphicsScene* escena;
    QGraphicsView* vista;

    QPushButton* botonNivel1;
    QPushButton* botonNivel2;
    QPushButton* botonNivel3;

    QGraphicsTextItem* titulo;
    QGraphicsTextItem* descripcion1;
    QGraphicsTextItem* descripcion2;
    QGraphicsTextItem* descripcion3;
};

#endif // MAINWINDOW_H
