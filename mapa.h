#pragma once
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QImage>

namespace Ui {
class Mapa;
}

class Mapa : public QWidget
{
    Q_OBJECT

public:
    explicit Mapa(QWidget *parent = 0);
    ~Mapa();

    void setMapa(QImage map);

private:
    Ui::Mapa *ui;
    QImage imagen;
    void paintEvent(QPaintEvent *e);
    QImage mapa;

private slots:
    void slot_apretoVolver();
};
