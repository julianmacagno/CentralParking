#include "mapa.h"
#include "ui_mapa.h"

Mapa::Mapa(QWidget *parent) :    QWidget(parent),    ui(new Ui::Mapa)
{
    ui->setupUi(this);

    imagen.load(":/park1.png");
    this->setWindowTitle("Parking");
    setWindowIcon(QIcon(":/park2.png"));

    connect(this->ui->pbVolver, SIGNAL(clicked()), this, SLOT(slot_apretoVolver()));
}

Mapa::~Mapa()
{
    delete ui;
}

void Mapa::setMapa(QImage map)
{
    mapa = map;
}

void Mapa::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawImage(0,0,imagen.scaled(this->width(),this->height()));
    painter.drawImage(20,50,mapa.scaled(this->width()-40,this->height()-110));
}

void Mapa::slot_apretoVolver()
{
    this->hide();
}
