#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :    QWidget(parent),    ui(new Ui::Widget)
{
    ui->setupUi(this);
    mapa=NULL;

    this->manager = new DataManager();
    this->gpsManager = new PosManager();

    imagen.load(":/park1.png");
    this->setWindowTitle("Parking");
    setWindowIcon(QIcon(":/park2.png"));

    connect(this->ui->pbMapa, SIGNAL(clicked()), this, SLOT(slot_apretoMapa()));
    connect(this->ui->pbDireccion, SIGNAL(clicked()), this, SLOT(slot_apretoDireccion()));
    connect(this->ui->pbGps, SIGNAL(clicked()), this, SLOT(slot_apretoGps()));
    connect(this->manager, SIGNAL(receivedCoordinates(QList<QString>)), this, SLOT(slot_recibirCoordenadas(QList<QString>)));
    connect(this->manager, SIGNAL(receivedSites(QList<DataManager::Sitio>)), this, SLOT(slot_recibirSitios(QList<DataManager::Sitio>)));
    connect(this->ui->leDireccion,SIGNAL(returnPressed()),this,SLOT(slot_apretoDireccion()));
    connect(this->manager,SIGNAL(receivedRoute(QList<QString>)),this,SLOT(slot_recibirRuta(QList<QString>)));
    connect(this->manager,SIGNAL(receivedMap(QImage)),this,SLOT(slot_recibirMapa(QImage)));
    connect(this->ui->lista,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(slot_itemClickeado(QListWidgetItem*)));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawImage(0,0,imagen.scaled(this->width(),this->height()));
}

void Widget::slot_apretoDireccion()
{
    this->manager->getCoordinates(this->ui->leDireccion->text());
}

void Widget::slot_apretoGps()
{
    QStringList a = gpsManager->getLatLongGPS();
    QList<QString> b;
    b.append(a.at(0));
    b.append(a.at(1));
    manager->lat = b.at(0).toDouble();
    manager->lng = b.at(1).toDouble();
    this->manager->getSites("parking", b);
}

void Widget::slot_recibirCoordenadas(QList<QString> l)
{
    this->manager->getSites("parking", l);
}

void Widget::slot_recibirSitios(QList<DataManager::Sitio> l)
{
    ui->lista->clear();
    for(int i=0; i<l.size();i++)
    {
        QListWidgetItem *a = new QListWidgetItem;
        a->setData(100,l.at(i).lat);
        a->setData(101,l.at(i).lng);
        a->setText(l.at(i).nombre + "\n" + "Puntuacion: " + l.at(i).rating + "\n\n");

        ui->lista->addItem(a);
    }
}

void Widget::slot_recibirRuta(QList<QString> l)
{
    this->manager->getMap(l);
}

void Widget::slot_recibirMapa(QImage map)
{
    if(!mapa)
    {
        mapa = new Mapa;
    }

    mapa->setMapa(map);
    mapa->show();
}

void Widget::slot_apretoMapa()
{
    if(mapa)
    {
        mapa->show();
    }
}

void Widget::slot_itemClickeado(QListWidgetItem *item)
{
    manager->getRoute(QString::number(manager->lat),QString::number(manager->lng),item->data(100).toString(),item->data(101).toString());
}
