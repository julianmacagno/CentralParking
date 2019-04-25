#include "datamanager.h"
#include <QFile>

DataManager::DataManager()
{
    managerCoordinates = new QNetworkAccessManager(this);
    managerSites = new QNetworkAccessManager(this);
    managerRoutes = new QNetworkAccessManager(this);
    managerMap = new QNetworkAccessManager(this);

    connect(managerSites,SIGNAL(finished(QNetworkReply*)),this,SLOT(slot_getSites(QNetworkReply *)));
    connect(managerCoordinates,SIGNAL(finished(QNetworkReply*)),this,SLOT(slot_getCoordinates(QNetworkReply *)));
    connect(managerRoutes,SIGNAL(finished(QNetworkReply*)),this,SLOT(slot_getRoutes(QNetworkReply*)));
    connect(managerMap,SIGNAL(finished(QNetworkReply*)),this,SLOT(slot_getMap(QNetworkReply*)));
}

DataManager::~DataManager()
{
}

void DataManager::getCoordinates(QString street)
{
    QString base = "https://maps.googleapis.com/maps/api/geocode/json?address=";

    base += street;
    base += "&key=AIzaSyCnRcVd391QIPAa7i7tkfHrpP-BuehQ5FE";


    managerCoordinates->get(QNetworkRequest(QUrl(base)));
}

void DataManager::getSites(QString search, QList<QString> coord)
{
    QString base = "https://maps.googleapis.com/maps/api/place/nearbysearch/json?location=";
    base.append(coord.at(0)+","+coord.at(1)+"&rankby=distance&types="+search);
    base.append("&key=AIzaSyCWkS32Vvd6wBepLJkNJfRxELfXQOw8NFg");

    managerSites->get(QNetworkRequest(QUrl(base)));
}

void DataManager::getRoute(QString latO, QString lngO, QString latD, QString lngD)
{
    QString base = "https://maps.googleapis.com/maps/api/directions/json?";
    QString params = "origin=" + latO + "," + lngO + "&destination=" + latD + "," + lngD;
    params += "&key=AIzaSyC5B1slkQH2LvD3kB3Ifsin47Tex52cri0";
    base += params;
    managerRoutes->get(QNetworkRequest(QUrl(base)));
}

void DataManager::getMap(QList<QString> coords)
{
    QString base = "https://maps.googleapis.com/maps/api/staticmap?";
    QString params = "&size=360x640&zoom=16&path=";

    for(int i=0;i<coords.size();i++)
    {
        params = params + coords.at(i) + "," + coords.at(i+1) + "|";
        i++;
    }

    params.chop(1);
    base += params;
    managerMap->get(QNetworkRequest(QUrl(base)));
}

void DataManager::slot_getCoordinates(QNetworkReply *reply)
{
    QList<QString> list;
    QJsonDocument a = QJsonDocument::fromJson(reply->readAll());
    QJsonObject b = a.object().value("results").toArray().first().toObject();
    QJsonValue e = b.value("geometry").toObject().value("location");
    lat = e.toObject().take("lat").toDouble();
    list.append(QString::number(lat));
    lng = e.toObject().take("lng").toDouble();
    list.append(QString::number(lng));

    emit receivedCoordinates(list);
}

void DataManager::slot_getSites(QNetworkReply *reply)
{
    QList<Sitio> list;
    QJsonDocument a = QJsonDocument::fromJson(reply->readAll());
    QJsonArray b = a.object().value("results").toArray();

    //COMENTAR PARA DEVOLVER LISTA DE CALLES
    for(int i=0;i<b.size();i++)
    {
        Sitio sitio;
        QJsonValue geo = b.at(i).toObject().take("geometry");

        QJsonObject loc = geo.toObject().take("location").toObject();

        sitio.lat = QString::number(loc.take("lat").toDouble());
        sitio.lng = QString::number(loc.take("lng").toDouble());

        sitio.nombre = b.at(i).toObject().take("name").toString();
        sitio.rating = QString::number(b.at(i).toObject().take("rating").toDouble());
        if(sitio.rating == "0")
            sitio.rating = "No Disponible";

        list.append(sitio);
    }

    //DESCOMENTAR PARA DEVOLVER LISTA DE CALLES
    /*for(int i=0; i<b.size();i++)
    {
        QJsonValue c = b.at(i).toObject().take("vicinity");
        QString d = c.toString();

        for(QString::Iterator j = d.begin(); j!=d.end(); j++)
        {
            if(j->digitValue() != -1)
            {
                list.append(d);
                break;
            }
        }
    }*/

    emit receivedSites(list);
}

void DataManager::slot_getRoutes(QNetworkReply *reply)
{
    QList<QString> list;

    QJsonDocument a = QJsonDocument::fromJson(reply->readAll());
    QJsonArray b = a.object().value("routes").toArray();
    for(int i=0; i<b.size();i++)
    {
        QJsonArray c = b.at(i).toObject().value("legs").toArray();

        for(int i=0;i<c.size();i++)
        {
            QJsonArray d = c.at(i).toObject().value("steps").toArray();

            QJsonObject e = d.at(0).toObject().value("start_location").toObject();
            list.append(QString::number(e.value("lat").toDouble()));
            list.append(QString::number(e.value("lng").toDouble()));

            e = d.at(0).toObject().value("end_location").toObject();
            list.append(QString::number(e.value("lat").toDouble()));
            list.append(QString::number(e.value("lng").toDouble()));

            for(int i=1;i<d.size();i++)
            {
                e = d.at(i).toObject().value("end_location").toObject();
                list.append(QString::number(e.value("lat").toDouble()));
                list.append(QString::number(e.value("lng").toDouble()));
            }
        }
    }

    emit receivedRoute(list);
}

void DataManager::slot_getMap(QNetworkReply *reply)
{
    QImage imagen = QImage::fromData(reply->readAll());

    emit receivedMap(imagen);
}
