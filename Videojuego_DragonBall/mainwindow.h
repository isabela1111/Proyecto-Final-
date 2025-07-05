#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QPushButton>
#include "nivel1.h"
#include "nivel2.h"
#include "nivel3.h"
#include <QMediaPlayer>
#include <QAudioOutput>
#include "recursos.h"

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

    Nivel1* nivel1;
    Nivel2* nivel2;
    Nivel3* nivel3;

    QMediaPlayer* reproductorIntro;
    QAudioOutput* salidaAudio;


private slots:
    void iniciarNivel1();
    void iniciarNivel2();
    void iniciarNivel3();
};

#endif // MAINWINDOW_H
