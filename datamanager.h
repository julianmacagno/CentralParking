#pragma once
#include<QNetworkRequest>
#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<QUrl>
#include<QDebug>
#include<QObject>
#include<QString>
#include<QList>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonValue>
#include<QJsonArray>
#include<QImage>
#include<QWidget>

class DataManager : public QObject
{
    Q_OBJECT

public:
    DataManager();
    ~DataManager();

    struct Sitio {
        QString lat, lng, nombre, rating;
    };

    /**
     * @brief solicita una consulta get las coordenadas de una direccion
     * en la api de Geolocation de Google Maps.
     * @param street: direcciones de las cual se quiere obtener sus coordenadas
     */
    void getCoordinates(QString street);

    /**
     * @brief solicita en una consulta get informacion relacionada a una busqueda
     * en la api de Sites de Google Maps.
     * @param search: el tipo de sitio que se quiere buscar.
     * @param coord: coordendas de la ubicacion respecto a la cual se realiza la busqueda.
     */
    void getSites(QString search, QList<QString> coord);

    /**
     * @brief solicita en una consulta get las indicaciones para llegar desde el
     * punto lat al punto long. La encargada de procesarlas sera slot_getRoute();
     * @param latO latitud del origen
     * @param lngO longitud del origen
     * @param latD latitud del destino
     * @param lngD longitud del destino
     */
    void getRoute(QString latO, QString lngO, QString latD, QString lngD);

    /**
     * @brief solicita en una consulta get una imagen de un mapa con la ruta
     * trazada que se desea seguir. Encargado de procesarla sera slot_getMapa();
     * @param coords lista de coordenadas a seguir en el trazado de la ruta.
     */
    void getMap(QList<QString> coords);

private:
    QNetworkAccessManager *managerCoordinates, *managerSites, *managerRoutes, *managerMap;

public:
    double lat, lng;

private slots:
    /**
     * @brief recibe y procesa una respuesta json de las coordenadas asociadas a
     * una direccion. Devuelve las mismas en un signal receivedCoordinates().
     * @param reply
     */
    void slot_getCoordinates(QNetworkReply *reply);

    /**
     * @brief recibe y procesa una respuesta json con informacion asociada a lugares
     * proximos a la direccion parametrizada en la funcion getSites().
     * Devuelve los mismos en un signal receivedSites().
     * @param reply
     */
    void slot_getSites(QNetworkReply *reply);

    /**
     * @brief recibe y procesa una respuesta json con informacion relacionada a las
     * indicaciones y coordenadas de una ruta para llegar desde la ubicacion destino
     * y la ubicacion origen parametrizadas en la funcion getRoute().
     * Devuelve una lista de coordenadas en una signal receivedRoute().
     * @param reply
     */
    void slot_getRoutes(QNetworkReply *reply);

    /**
     * @brief recibe y procesa una respuesta que contendra la imagen con la ruta dibujada
     * para llegar desde el destino al origen en la ruta parametrizada por la funcion
     * getRoute(). Devuelve la misma en una signal receivedMap().
     * @param reply
     */
    void slot_getMap(QNetworkReply *reply);

signals:
    /**
     * @brief devuelve un QList<QString> con las dos coordenadas asociadas a la direccion
     * parametrizada por la funcion getCoordinates.
     */
    void receivedCoordinates(QList<QString>);

    /**
     * @brief devuelve en un QList<Sitios> informacion asociada con la lista de lugares
     * proximos a la ubicacion parametrizada en la funcion getSites().
     * Dicha info es un Struct con los atributos latitud, longitud, nombre y puntuacion.
     */
    void receivedSites(QList<DataManager::Sitio>);

    /**
     * @brief devuelve una lista de coordendas correspondientes a las indicaciones de
     * una ruta especifica. Dichas coordendas deben ser tomada de dos en dos como latitud
     * y longitud respectivamente.
     */
    void receivedRoute(QList<QString>);

    /**
     * @brief devuelve una Imagen de un mapa con una ruta trazada.
     */
    void receivedMap(QImage);
};
