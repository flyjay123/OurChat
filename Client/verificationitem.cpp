#include "verificationitem.h"
#include "ui_verificationitem.h"

VerificationItem::VerificationItem(VerifyInfo _info,TcpClient* _socket, int _type,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VerificationItem)
{
    type = _type;
    ui->setupUi(this);
    info = _info;
    ui->label_name->setText(info.name);
    ui->label_name_msg->setText(info.msg);
    socket = _socket;
}

VerificationItem::~VerificationItem()
{
    delete ui;
}

void VerificationItem::on_comboBox_currentIndexChanged(const QString &arg1)
{
    ui->pushButton->setText(arg1);
}

void VerificationItem::on_pushButton_clicked()
{
    json msg = {{"cmd",     cmd_add_friend_response},
                {"name",    info.name},
                {"msg",     info.msg,},
                {"account", info.account},
                {"sender",  info.sender},
                {"reply",   "yes"}};
    if (type)
    {
        msg["cmd"] = cmd_group_join_response;
        msg["groupName"] = info.groupName;
    }
    if(ui->comboBox->currentIndex())
    {
        msg["reply"] = "no";
    }
    socket->SendMsg(msg);
}
