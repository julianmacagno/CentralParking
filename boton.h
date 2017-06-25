#pragma once
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QImage>

namespace Ui {
class Boton;
}

class Boton : public QWidget
{
    Q_OBJECT

public:
    explicit Boton(QWidget *parent = 0);
    ~Boton();

private:
    Ui::Boton *ui;
    QImage Img;

    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);

signals:
    void clicked();
};
