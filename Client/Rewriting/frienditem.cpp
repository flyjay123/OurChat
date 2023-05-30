#include "frienditem.h"
#include "ui_frienditem.h"
#include "stringtool.h"
#include <QDebug>
#include "client.h"
#include "stringtool.h"
#include <QSslSocket>

FriendInfoWidget *FriendItem::curWidget = nullptr;
FriendItem::FriendItem(FriendInfo _info, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendItem)
{
    ui->setupUi(this);
    SetInfo(_info);
    m_type = 0;
    m_account = _info.account;
    m_name = _info.name;
    Init();
}

FriendItem::FriendItem(GroupInfo _info, QWidget *parent) :
        QWidget(parent),
        ui(new Ui::FriendItem)
{
    ui->setupUi(this);
    SetInfo(_info);
    m_type = 1;
    m_account = _info.groupAccount;
    m_name = _info.groupName;
    Init();

}

void FriendItem::Init() {
    ui->lineEdit_newMsg->hide();
    timer = new QTimer;
    timer->setInterval(1000);
    if(m_type)
        widget = new  FriendInfoWidget(groupInfo);
    else
        widget = new  FriendInfoWidget(info);
    curWidget = widget;
    connect(timer,&QTimer::timeout,this,[=](){
        widget->close();
    });

    connect(ui->label_icon,&FriendIconLabel::enterIconLabel,this,&FriendItem::showInfoWidget);
    connect(ui->label_icon,&FriendIconLabel::leaveIconLabel,this,&FriendItem::closeInfoWidget);
    connect(widget,&FriendInfoWidget::enterWidget,this,[&](){timer->stop();});
    connect(widget,&FriendInfoWidget::leaveWidget,this,[&](){timer->start();});
    ui->label_icon->SetIcon(info.icon);
}

FriendItem::~FriendItem()
{
    delete ui;
}

void FriendItem::setName(const QString &name)
{
    ui->label_name->setText(name);
}

void FriendItem::setSignature(const QString &signature)
{
    ui->label_sig->setText(signature);
}
void FriendItem::setAvatar(const QPixmap &avatar)
{
    ui->label_icon->setPixmap(avatar);
}

void FriendItem::SetNewMsgNum(int num)
{
    ui->lineEdit_newMsg->setText(StringTool::Int2QStr(num));
}

void FriendItem::NewMsgPlusOne()
{
    QString a = ui->lineEdit_newMsg->text();
    QString text;

    if (a == "99+") {
        return;
    }
    else if(a.toInt() >= 99)
    {
        text = StringTool::Int2QStr(99) + "+";
    }
    else
        text = StringTool::Int2QStr(a.toInt()+1);
     ui->lineEdit_newMsg->setText(text);
}

void FriendItem::SetLastMsg(const QString& msg)
{
    ui->label_sig->setText(msg);
}

void FriendItem::SetInfo(FriendInfo _info)
{
    info = _info;
    m_account = _info.account;
    ui->label_sig->setText(info.sig);
    ui->label_name->setText(info.name);
}

void FriendItem::SetInfo(GroupInfo _info)
{
    groupInfo = _info;
    m_account = _info.groupAccount;
    ui->label_name->setText(groupInfo.groupName);
}

QString FriendItem::getLabelName()
{
    return  ui->label_name->text();
}

void FriendItem::on_lineEdit_newMsg_textChanged(const QString &arg1)
{
    qDebug()<<"show or hide";
   int a = arg1.toInt();
   if(a == 0)
   {
       ui->lineEdit_newMsg->hide();
   }
   else
       ui->lineEdit_newMsg->show();
}

void FriendItem::showInfoWidget()
{
    if(!curWidget->isHidden())
    {
        if(curWidget == widget)
            return;
        curWidget->close();
    }
    QPoint labelPos = ui->label_icon->pos();
    QPoint globalPos = this->mapToGlobal(QPoint(0,0)) + labelPos;
    int x = globalPos.x();
    if(x>(widget->width()+100))
        globalPos.setX(x-widget->width()-100);
    else
         globalPos.setX(this->parentWidget()->width()+this->mapToGlobal(QPoint(0,0)).x());
    //globalPos.setY(globalPos.y()-widget->height());
    curWidget = widget;
    widget->move(globalPos);
    timer->stop();
    widget->show();
        //qDebug() << "x:" << StringTool::Int2QStr(globalPos.x()) << "y:" << StringTool::Int2QStr(globalPos.y());


}

void FriendItem::closeInfoWidget()
{
    timer->start();
}

