#include "imagegenerator.h"

#include <QPixmap>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QUrlQuery>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QObject>
#include <QDebug>
#include <QMap>







ImageGenerator::ImageGenerator() {
    connect(&manager, &QNetworkAccessManager::finished, this, &ImageGenerator::onReplyFinished);
    connect(&timer, &QTimer::timeout, this, &ImageGenerator::checkStatus);
}
// запрос на модель. пока что модель одна. model_id =4;
void ImageGenerator::getmodel() {
    QNetworkRequest request(QUrl(URL + "key/api/v1/models"));
    request.setRawHeader("X-Key", ("Key " + apiKey).toUtf8());
    request.setRawHeader("X-Secret", ("Secret " + secretKey).toUtf8());
    manager.get(request);
}
// запрос на статус генерации картинки.
void ImageGenerator::checkStatus() {
    QNetworkRequest request(QUrl(URL + "key/api/v1/text2image/status/" + uuid));
    request.setRawHeader("X-Key", ("Key " + apiKey).toUtf8());
    request.setRawHeader("X-Secret", ("Secret " + secretKey).toUtf8());
    manager.get(request);
}
// функция обработки ответов сервера и ошибок
void ImageGenerator::onReplyFinished(QNetworkReply* reply) {
    if (reply->error())
    {
        QMessageBox *box = new QMessageBox;                                        // обработка сообщений об ошибках
        box->setText(reply->errorString());
        box->show();
        return;
    };
    QByteArray const response = reply->readAll();
    QJsonDocument const Doc = QJsonDocument::fromJson(response);
    QString const url=reply->url().toString();
    if (url.contains("/models")) {                                                 // Обработка model_id
        QJsonArray const dataJson = Doc.array();
        if (!dataJson.isEmpty()) {
            int const model_id = dataJson[0].toObject().value("id").toInt();
            sendGenerateRequest(model_id);
        }
    } else if (url.contains("/text2image/run")) {                                  // Обработка результата генерации
        uuid = Doc.object().value("uuid").toString();
        if (!uuid.isEmpty()) {
            timer.start(3000);                                                     // Установка интервала для проверки статуса
        }
    } else if (url.contains("/text2image/status")) {                               // Обработка статуса
        QJsonObject const Object = Doc.object();
        QString const status = Doc.object().value("status").toString();
        if (status == "DONE") {
            QString base64Image = Object.value("images").toArray()[0].toString();
            QByteArray byteArray = QByteArray::fromBase64(base64Image.toUtf8());
            Image.loadFromData(byteArray);
            Pix = QPixmap::fromImage(Image);
            timer.stop();
            emit set_pix(Pix);
        } else if (status == "FAIL") {
            QMessageBox *messagebox = new QMessageBox;
            messagebox->setText("Image generation failed.");
            timer.stop();
            messagebox->show();
       } /*else {
            qDebug() << status;                                                 // при желании можно печатать статус каждые timer(ms)
        }*/
    }
reply->deleteLater();
};
// создание запроса со всеми характеристиками
void ImageGenerator::sendGenerateRequest(int model_id = 4) {
    QNetworkRequest request(QUrl(this->URL + "key/api/v1/text2image/run"));
    request.setRawHeader("X-Key", ("Key " + this->apiKey).toUtf8());
    request.setRawHeader("X-Secret", ("Secret " + this->secretKey).toUtf8());
    // Тело запроса
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    // Часть с model_id
    QHttpPart modelPart;
    modelPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"model_id\""));
    modelPart.setBody(QByteArray::number(model_id));  // model_id
    multiPart->append(modelPart);
    // Часть с params (JSON)
    QJsonObject generateParams;
    generateParams["query"] = query;
    QJsonObject params;
    params["type"] = type;
    params["style"] = style;
    params["numImages"] = numImages;
    params["width"] = wigth;
    params["height"] = height;
    params["generateParams"] = generateParams;
    params["negativePromptUnclip"] = negative;
    // Преобразование параметров в JSON
    QJsonDocument jsonDoc(params);
    QByteArray jsonData = jsonDoc.toJson();
    QHttpPart paramsPart;
    paramsPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"params\""));
    paramsPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    paramsPart.setBody(jsonData);
    multiPart->append(paramsPart);
    manager.post(request, multiPart);
};
ImageGenerator::~ImageGenerator(){};



void ImageGenerator::get(int size, int parametr){
    switch(parametr){
    case 1: {this->wigth = size; break;}
    case 2: {this->height = size; break;}
    default: {break;}
    }
}

void ImageGenerator::get(QString text, int parametr){
    switch(parametr){
    case 1: {this->query = text; break;}
    case 2: {this->negative = text; break;}
    case 3: {this->style = text; break;}
    default: {break;}
    }
}


