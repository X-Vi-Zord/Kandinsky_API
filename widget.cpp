#include "widget.h"
#include "ui_widget.h"
#include "imagegenerator.h"

#include <QObject>
#include <QValidator>
#include <QEventLoop>



Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->line_height->setValidator(new QIntValidator(0, 1024,this));
    ui->line_width->setValidator(new QIntValidator(0, 1024,this));
    // запрос актуальных стилей и их размещение в интерфейсе
    fetchStyles();
    ui->comboBox->addItems(styles.keys());
    //подготовка окна под изображение
     ui->for_picture->resize(512,512);
}
Widget::~Widget()
{
    delete ui;
}
// информация для запроса
void Widget::on_pushButton_clicked()
{
    ImageGenerator *gen = new ImageGenerator();
    connect(gen,&ImageGenerator::set_pix,this,&Widget::get_pix);
    gen->get(ui->text_request->toPlainText(),1);
    gen->get(ui->negative_request->toPlainText(),2);
    gen->get(styles[ui->comboBox->currentText()],3);
    // обработка на ошибки размеров страницы
    if(ui->line_width->text().toInt() <= 1024) {
        gen->get(ui->line_width->text().toInt(),1);
    }
    if(ui->line_height->text().toInt() <= 1024) {
        gen->get(ui->line_height->text().toInt(),2);
    }
    gen->getmodel(); // запрос модели и генерации
}
// генератор картинки
void Widget::get_pix(QPixmap pix){
    ui->for_picture->resize(pix.size());// по желанию
    ui->for_picture->setPixmap(pix);
}
// запрос актуальных стилей
void Widget::fetchStyles(){
QNetworkRequest request(QUrl("https://cdn.fusionbrain.ai/static/styles/key"));
QEventLoop loop;
QNetworkAccessManager manager;
connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
QNetworkReply *reply = manager.get(request);
loop.exec();
    QJsonArray const J_Styles =QJsonDocument::fromJson(reply->readAll()).array();
    for (const auto& style : J_Styles) {
        styles.insert(style.toObject().value("title").toString(), style.toObject().value("name").toString());
    }
};

