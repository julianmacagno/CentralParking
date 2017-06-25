#include "boton.h"
#include "ui_boton.h"

Boton::Boton(QWidget *parent) :    QWidget(parent),    ui(new Ui::Boton)
{
    ui->setupUi(this);
    repaint();
    Img.load(":/boton.png");
}

Boton::~Boton()
{
    delete ui;
}

void Boton::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawImage(0,0,Img.scaled(this->width(),this->height()));
}

void Boton::mousePressEvent(QMouseEvent *e)
{
    emit clicked();
}
