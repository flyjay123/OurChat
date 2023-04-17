#include "verificationitem.h"
#include "ui_verificationitem.h"

VerificationItem::VerificationItem(VerifyInfo _info,TcpClient* _socket, int _type,QWidget *parent) :
    QWidget(parent),ui(new Ui::VerificationItem),info(_info),socket(_socket)
{
    type = _type;
    ui->setupUi(this);
    ui->label_name->setText(info.name);
    ui->label_name_msg->setText(info.msg);
    ui->label_icon->SetIcon(info.icon);
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
        ui->pushButton->setText("已拒绝");
        msg["reply"] = "no";
    }
    else
        ui->pushButton->setText("已同意");
    ui->pushButton->setEnabled(false);
    socket->SendMsg(msg);
}
