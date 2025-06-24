#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QGraphicsScene* escena;
    QGraphicsTextItem* titulo;
    QPushButton* botonNivel1;
    QPushButton* botonNivel2;
    QPushButton* botonNivel3;
    QGraphicsTextItem* descripcion1;
    QGraphicsTextItem* descripcion2;
    QGraphicsTextItem* descripcion3;

private slots:
    void iniciarNivel1();
    void iniciarNivel2();
    void iniciarNivel3();
};

#endif // MAINWINDOW_H
