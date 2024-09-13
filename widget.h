#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMap>
#include <QNetworkAccessManager>


QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:

    Widget(QWidget *parent = nullptr);
    ~Widget();
    void fetchStyles();                     // проверка допустимых стилей

public slots:

    void get_pix(QPixmap);                  // получение Pixmap из ImageGenerator

private slots:

    void on_pushButton_clicked();

    void on_textEdit_textChanged();

private:

    QNetworkAccessManager const manager;
    QMap<QString, QString> styles;          // Стили + имена стилей для post запроса
    Ui::Widget *ui;

};
#endif // WIDGET_H
