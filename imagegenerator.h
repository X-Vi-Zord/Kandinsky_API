#pragma once
#include <QPixmap>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QUrlQuery>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QObject>
#include <QImage>
#include <QMap>







class ImageGenerator : public QObject {
    Q_OBJECT

public:

    ImageGenerator();
    ~ImageGenerator();

    void getmodel();

    void generate();

    void checkStatus();

    void get(int,int);

    void get(QString,int);

    QPixmap Pix;                    // Картинка, которую получаем

private slots:

    void onReplyFinished(QNetworkReply*);   // обработка ответов

    void sendGenerateRequest(int);          // отправка запроса на картинку с номером модели


private:

    QNetworkAccessManager manager;
    QString uuid;                           // индивидуальный номер запроса
    QTimer timer;                           // для регулярного уточнения состояния картинки

    QImage Image;                           // промежуточный файл-> декодируется из reply (base64)

    const QString URL = "https://api-key.fusionbrain.ai/";      // ссылка для запросов
    // вставьте ключи, полученные при регистрации на fusion brain
    const QString apiKey = "Your_key";
    const QString secretKey = "Your_secret";




    // параметры генерации см. https://fusionbrain.ai/docs/doc/api-dokumentaciya/
    QString query = "Black cat";
    QString const type = "GENERATE";
    int const numImages = 1;
    int wigth = 512;
    int height = 512;
    QString negative;
    QString style;

signals:

    void set_pix(QPixmap);

};
