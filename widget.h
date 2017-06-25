#ifndef WIDGET_H
#define WIDGET_H

#include "datamanager.h"
#include "posmanager.h"
#include "mapa.h"
#include <QWidget>
#include <QListWidgetItem>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    QImage imagen;
    Mapa *mapa;
    DataManager *manager;
    PosManager *gpsManager;
    void paintEvent(QPaintEvent *e);


private slots:
    void slot_apretoDireccion();
    void slot_apretoGps();
    void slot_recibirCoordenadas(QList<QString> l);
    void slot_recibirSitios(QList<DataManager::Sitio> l);
    void slot_recibirRuta(QList<QString> l);
    void slot_recibirMapa(QImage map);
    void slot_apretoMapa();
    void slot_itemClickeado(QListWidgetItem *item);
};

#endif // WIDGET_H
