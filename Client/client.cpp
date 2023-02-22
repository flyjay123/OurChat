#include "client.h"
#include "ui_client.h"
#include <QGraphicsDropShadowEffect>
#include <QGuiApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <windows.h>
#include <QMouseEvent>
#include <QDebug>
#include "addfriend.h"
#include "frienditem.h"
#include <QListWidget>
#include <QListWidgetItem>

Client::Client(SelfInfo info ,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);
    InitUI();
    selfInfo.name = info.name;
    selfInfo.account=info.account;
    selfInfo.password=info.password;

    ui->listWidget_info->setViewMode(QListWidget::ListMode); //显示模式

    connect(&t,&TcpClient::CallClient,this,&Client::ClientMsgHandler);
}

Client::~Client()
{
    delete ui;
}

void Client::RefreshFriendList()
{
    json msg;
    msg.insert("cmd","friend-list");
    msg.insert("account",QString("%1").arg(selfInfo.account));
    t.SendMsg(msg);
    t.socket->waitForReadyRead(200);
}

void Client::InitUI()
{
    this->setWindowTitle("OurChat");
//    int width = this->width()-10;
//    int height = this->height()-10;
//    ui->centerWidget->setGeometry(5,5,width,height);
//    ui->centerWidget->setStyleSheet("QWidget#centerWidget{ border-radius:4px; background:rgba(255,255,255,1); }");
    this->setWindowFlags(Qt::FramelessWindowHint);          //去掉标题栏无边框
    this->setAttribute(Qt::WA_TranslucentBackground,true);
    //实例阴影shadow
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    //设置阴影距离
    shadow->setOffset(0, 0);
    //设置阴影颜色
    shadow->setColor(QColor(39,40,43,100));
    //设置阴影圆角
    shadow->setBlurRadius(10);
    //给嵌套QWidget设置阴影
    ui->centerWidget->setGraphicsEffect(shadow);


    m_isfull = false;
}

void Client::mousePressEvent(QMouseEvent *event)
{
    mouseWindowTopLeft = event->pos();
}

void Client::mouseMoveEvent(QMouseEvent *event)
{
    //窗口移动
    if (event->buttons() & Qt::LeftButton)
    {
        mouseDeskTopLeft = event->globalPos();
        windowDeskTopLeft = mouseDeskTopLeft - mouseWindowTopLeft;  //矢量计算
        this->move(windowDeskTopLeft);     //移动到目的地
    }
}

void Client::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    if(m_isfull){
        //取消全屏
        m_isfull = false;
        ui->centerWidget->setGeometry(m_rect);

        ui->centerWidget->move(QApplication::desktop()->screen()->rect().center() - ui->centerWidget->rect().center());
    }
    else {
        m_isfull = true;
        m_rect = ui->centerWidget->rect();
        setGeometry(QGuiApplication::primaryScreen()->availableGeometry()); // 不包含windows任务栏区域
        ui->centerWidget->setGeometry(this->rect());
    }
}


void Client::on_pushBtn_max_clicked()
{
//    this->showFullScreen(); //全屏
    if(m_isfull){
        //取消全屏
        m_isfull = false;
        ui->centerWidget->setGeometry(640,480,m_rect.width(),m_rect.height());
        ui->centerWidget->move(QApplication::desktop()->screen()->rect().center() - ui->centerWidget->rect().center());
    }
    else {
        m_isfull = true;
        m_rect = ui->centerWidget->rect();
        setGeometry(QGuiApplication::primaryScreen()->availableGeometry()); // 不包含windows任务栏区域
        ui->centerWidget->setGeometry(this->rect());
    }
//    ui->centerWidget->showMaximized();
//    this->showMaximized();
}

void Client::on_pushBtn_close_clicked()
{
    this->close();
}

void Client::on_pushBtn_hide_clicked()
{
    QWidget* pWindow = this->window();
    if(pWindow->isTopLevel())
        pWindow->showMinimized();
}

void Client::on_pushButton_addFriend_clicked()
{
    AddFriend *add = new AddFriend();
    add->show();
}

void Client::on_pushBtn_refresh_clicked()
{
    RefreshFriendList();
}

void Client::ClientMsgHandler(json msg)
{
    qDebug() << "ClientMsgHandler" << endl;
    QString cmd = msg["cmd"].toString();
    if(cmd == "friend-list")
    {
        ui->listWidget_info->clear();
        QJsonArray list = msg["msglist"].toArray();
        for(int i=0;i<list.size()-1;i+=2)
        {
            //QString f = QString("[%1] [%2]").arg(list[i].toString(),list[i+1].toString());
            //ui->listWidget_info->addItem(f);
            FriendItem *item = new FriendItem();
            item->setAccount(list[i].toInt());
            item->setName(list[i+1].toString());
            item->setSignature("这个人很懒，什么都没有留下...");
            QListWidgetItem *listItem = new QListWidgetItem(ui->listWidget_info);
            listItem->setSizeHint(QSize(260,85));
            ui->listWidget_info->addItem(listItem);
            ui->listWidget_info->setItemWidget(listItem, item);
        }
    }
}



