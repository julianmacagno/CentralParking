#pragma once
#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QGeoPositionInfoSource>
#include <QTimer>

class PosManager : public QObject
{
    Q_OBJECT
public:

    explicit PosManager(QObject *parent = 0);

    ~PosManager();
    //get´s para obtener latitud y longuitd
    float getAltitud()
    {
        return alt;
    }

    float getLongitud()
    {
        return lon;
    }
    /**
     * @brief getLatLongGPS Devuelve la posicion que da el GPS
     * @return Un QStringList de solo 2 elementos con el primer elemento con la latitud y el segundo con la longitud.
     */
    QStringList getLatLongGPS();

    /**
     * @brief getDomicilioActual Devuleve el domicilio de donde esta este celular.
     * @return
     */
    void getDomicilioActual();

    /**
     * @brief pedirDomicilio Metodo que puede usarse desde otra clase, por ejemplo, Ventana, para que PosManager pida a Google
     * el domicilio correspondiente a la latitud y longitud. Dentro de este metodo se utilizara get de QNetworkAccessManager para
     * pedir a Google.
     * @param latitud
     * @param longitud
     */
    void pedirDomicilio(double latitud, double longitud);
    /**
     * @brief pedirLatitudLonguitud Metodo que puede usarse desde otra clase, por ejemplo, Ventana, para que PosManager pida a Google
     * la latitud y longuitud correspondiente a una direccion. Dentro de este metodo se utilizara get de QNetworkAccessManager para
     * pedir a Google.
     * @param latitud
     * @param longitud
     */
    void pedirLatitudLonguitud(QString dom);

private:
       QNetworkAccessManager *manager, *manager1;

       //variables para guardar las coordenadas
       double alt;
       double lon;

signals:
        /**
         * @brief signal_domicilioObtenido Se emitira desde el slot_solicitudFinalizada luego de haber analizado el XML o JSON, y
         * obtenido el domiciio. Esta signal podrá ser conectada desde otra clase, por ejemplo Ventana
         * @param domicilio
         */
     void signal_domicilioObtenido(QString domicilio);

     void signal_coordenadasObtenidas(QString a , QString b);

private slots:
        /**
         * @brief slot_solicitudFinalizada Slot que se conectara desde la signal finished(QNetworkReply*), el cual traera la respuesta
         * de Google en XML o JSON, la cual se debera analizar para obtener el domicilio
         */
     void slot_solicitudFinalizada(QNetworkReply *);
     /**
      * @brief slot_solicitudLatLongFinalizada Slot que se conectara desde la signal finished(QNetworkReply*), el cual traera la respuesta
      * de Google en XML o JSON, la cual se debera analizar para obtener la latitud y longuitud
      */
     void slot_solicitudLatLongFinalizada(QNetworkReply *reply);

     //slot para actualizar la posicion.
     void positionUpdated(const QGeoPositionInfo &info);

};
