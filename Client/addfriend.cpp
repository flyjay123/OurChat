#include "addfriend.h"
#include "ui_addfriend.h"
#include <QGraphicsDropShadowEffect>


AddFriend::AddFriend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddFriend)
{
    ui->setupUi(this);
    Init();
    t = new TcpClient(this);
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
    if(t->waitForReadyRead(3000))
        {
            //t.onReadyRead();
            json j = t->GetMessage();
            if(j.isEmpty()) return;
            if(j["cmd"] == "yes")
            {
                list.clear();
                ui->listWidget->clear();
                QJsonArray arr =  j["msglist"].toArray();
                for(int i =0;i<arr.size();i++)
                {
                        QJsonArray a = arr[i].toArray();
                         list.push_back(a[0].toString());
                         list.push_back(a[1].toString());
               }
                for(int i =0;i<list.size()-1;i+=2)
                {
                    ui->listWidget->addItem(QString("[%1] [%2]").arg(list[i],list[i+1]));
                }
            }
        }
    else
    {
        qDebug() << "与服务器连接失败" << endl;
    }
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




