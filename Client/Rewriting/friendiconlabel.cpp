#include "friendiconlabel.h"
#include "stringtool.h"
#include <QDebug>

FriendIconLabel::FriendIconLabel(QWidget *parent) : QLabel(parent)
{
}

void FriendIconLabel::SetLocalIcon(QString url)
{
    this->setStyleSheet("border-image: url(" + url + ");");
}

void FriendIconLabel::SetHttpIcon(QString url)
{
    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    config.setProtocol(QSsl::TlsV1_1);

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkRequest request(url);
    QNetworkReply* reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=](){
        if(reply->error() == QNetworkReply::NoError)
        {
            QByteArray data = reply->readAll();
            QPixmap pixmap;
            pixmap.loadFromData(data);
            this->setPixmap(pixmap);
        }
        else
        {
            qDebug() << "Error:" << reply->errorString();
        }
        reply->deleteLater();
        manager->deleteLater();
    });
}

void FriendIconLabel::SetIcon(QString url)
{
    if(url.startsWith("http"))
    {
        SetHttpIcon(url);
    }
    else if(url.size())
    {
        SetLocalIcon(url);
    }
    else
    {
        setStyleSheet("border-image: url(:/Icons/src/QQIcon/icon1.jpg)");
    }
}

void FriendIconLabel::enterEvent(QEvent *event)
{
    Q_UNUSED(event);emit enterIconLabel();
}

void FriendIconLabel::leaveEvent(QEvent *event)
{

    Q_UNUSED(event);leaveIconLabel();
}

