#include "addfriend.h"
#include "ui_addfriend.h"
#include <QGraphicsDropShadowEffect>


AddFriend::AddFriend(TcpClient* fd,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddFriend)
{
    ui->setupUi(this);
    Init();
     t=fd;
     connect(t,&TcpClient::CallAddFriend,this,&AddFriend::CmdHandler);
}

AddFriend::~AddFriend()
{
    delete ui;
}

void AddFriend::Init()
{
    //this->setWindowFlags(Qt::FramelessWindowHint);          //去掉标题栏无边框
    //this->setAttribute(Qt::WA_TranslucentBackground,true);
    //实例阴影shadow
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    //设置阴影距离
    shadow->setOffset(0, 0);
    //设置阴影颜色
    shadow->setColor(QColor(39,40,43,100));
    //设置阴影圆角
    shadow->setBlurRadius(10);
    ui->textEdit->setPlaceholderText("验证信息");
    setWindowTitle("查找");

}

void AddFriend::on_pushButton_search_clicked()
{
    QString search = ui->lineEdit->text();
    json msg ={{"cmd","friend-search"},{"search-info",search}};
    t->SendMsg(msg);      
}

void AddFriend::on_pushButton_search_2_clicked()
{
    int row = ui->listWidget->currentRow();
    if(row >= 0)
    {
        json msg;
        msg.insert("cmd","addfriend-req");
        msg.insert("account",list[row*2]);
        msg.insert("sendmsg",ui->textEdit->toPlainText());
        t->SendMsg(msg);
    }
}

void AddFriend::CmdHandler(json msg)
{
    if(msg.isEmpty()) return;
    if(msg["cmd"] == "searchresult")
    {
        list.clear();
        ui->listWidget->clear();
        QJsonArray arr =  msg["msglist"].toArray();
        for(int i =0;i<msg["count"].toString().toInt();i++)
        {
                 list.push_back(arr[i].toObject().value("account").toString());
                 list.push_back(arr[i].toObject()["name"].toString());
       }
        for(int i =0;i<list.size()-1;i+=2)
        {
            ui->listWidget->addItem(QString("[%1] [%2]").arg(list[i],list[i+1]));
        }
    }
}




