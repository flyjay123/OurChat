#include "verificationitem.h"
#include "ui_verificationitem.h"

VerificationItem::VerificationItem(VerifyInfo _info,TcpClient* _socket,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VerificationItem)
{
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
    json msg = {{"cmd",cmd_add_friend_response},{"sender",info.sender},{"account",info.account},{"reply","yes"}};
    if(ui->pushButton->text() == "拒绝")
    {
        msg["reply"] = "no";
    }
    socket->SendMsg(msg);
}
