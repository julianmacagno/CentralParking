#include "posmanager.h"
#include <QGeoPositionInfoSource>
#include <QTimer>
PosManager::PosManager(QObject *parent) : QObject(parent)
{
    //Esta es la parte del gps que obtiene las coordenadas que indica el gps de el celular
    QGeoPositionInfoSource *source = QGeoPositionInfoSource::createDefaultSource(this);
    if (source) {
        connect(source, SIGNAL(positionUpdated(QGeoPositionInfo)),
                this, SLOT(positionUpdated(QGeoPositionInfo)));
        source->startUpdates();
    }
    qDebug()<<"Actualizar posicion GPS !"<<endl;

    qDebug()<<"Latitud: "<<alt;
    qDebug()<<"Longuitud"<<lon;

    manager = new QNetworkAccessManager;
    manager1 = new QNetworkAccessManager;

    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(slot_solicitudFinalizada(QNetworkReply *)));
    connect(manager1,SIGNAL(finished(QNetworkReply*)),this,SLOT(slot_solicitudLatLongFinalizada(QNetworkReply *)));
    qDebug()<<"<<<Constructor clase PosManager>>>"<<endl;
}

PosManager::~PosManager()
{
    qDebug()<<"SE INVOCO AL DESTRUCTOR";
}

void PosManager::slot_solicitudFinalizada(QNetworkReply *reply)
{
    //guardamos la respuerta en un QByteArray para acceder a los datos de forma sencillas con los indices
    QByteArray ba = reply->readAll();

    //obtengo los indices de las palabras que necesito que se encuentan en el xml que me devuelve la api de google maps
    int indiceComienzo = ba.indexOf("formatted_address");
    int indiceFin1 = ba.indexOf("X");
    int indiceFin2 = ba.indexOf("Córdoba");
    int inx1, inx2;

    qDebug() << "Indice comienzo domicilio : "<<  indiceComienzo;
    qDebug() << "Indice X: " << indiceFin1;
    qDebug() << "Indice Córdoba: " << indiceFin2;

    //aca preguntas si en la direccion del xml se encuenta una X, inx1 es igual indiceFin1, que es el indice de la X menos veinte - el indice del comienzo de la etiqueta formatted_address
    if(indiceFin1<indiceFin2)
    {
        inx1 = (indiceFin1 - 20) - indiceComienzo;
        qDebug() << "Diferencia indice inicio con x: "<< inx1;
    }
    //si no se encontro la x en la direccion, entonces el inx1 va a ser igual al indice de la palabra cordoba menos veinte menos el indice del comienzo de la etiqueta formatted_address
    else
    {
        inx1 =(indiceFin2 - 20) - indiceComienzo;
       qDebug() << "Diferencia indice inicio con Córdoba: "<< inx1;
    }
    inx2 =indiceComienzo + 18;
    QString direccion = ba.mid(inx2,inx1);

    //emito una señal de que ya se obtuvo el domicilio con el QString domicilio
    emit signal_domicilioObtenido(direccion);
}

void PosManager::slot_solicitudLatLongFinalizada(QNetworkReply *reply)
{
    QByteArray ba = reply->readAll();
    qDebug()<<ba;

    //obtengo los indices de las palabras que necesito que se encuentan en el xml que me devuelve la api de google maps
    int indiceComienzo = ba.indexOf("lat");
    int indiceFin = ba.indexOf("lng" );

    qDebug() << "Indice comienzo lat long : "<<  indiceComienzo;
    qDebug() << "Indice fin latitud longuitud: " << indiceFin;

    QString latitud = ba.mid(indiceComienzo +7,10);

    QString longuitud = ba.mid(indiceFin +6,10);

    qDebug()<<latitud;
    qDebug()<<longuitud;
    //emito una señal de que ya se obtuvieron las coordenadas.
    emit signal_coordenadasObtenidas(latitud,longuitud);
}

void PosManager::positionUpdated(const QGeoPositionInfo &info)
{
    alt=info.coordinate().latitude();
    lon=info.coordinate().longitude();
}

void PosManager::pedirDomicilio(double ll, double lg)
{
    qDebug()<<"TRAER XML PARA PEDIR EL DOMICILIO";

    QString a = QString::number(ll);
    QString b = QString::number(lg);

    qDebug()<<a;
    qDebug()<<b;

    manager->get(QNetworkRequest(QUrl("https://maps.googleapis.com/maps/api/geocode/xml?latlng=" + a + "," + b+ "&key=AIzaSyAiMFncByo2gYEkkUepKtVFsXLOZIu6TIw")));
}

void PosManager::pedirLatitudLonguitud(QString dom)
{
    qDebug()<<"TRAER XML PARA PEDIR LATITUD Y LONGUITD";

    QString a = dom;

    manager1->get(QNetworkRequest(QUrl("https://maps.googleapis.com/maps/api/geocode/json?address="+ a +"Córdoba&key=AIzaSyAiMFncByo2gYEkkUepKtVFsXLOZIu6TIw")));
}

QStringList PosManager::getLatLongGPS()
{
      QStringList cord;
      QString a = QString::number(alt);
      QString b = QString::number(lon);
      cord << a << b;
      qDebug() << cord;
      return cord;
}

void PosManager::getDomicilioActual()
{
    QString a = getLatLongGPS().at(0);
    QString b = getLatLongGPS().at(1);
    pedirDomicilio(a.toDouble(),b.toDouble());
}
