#ifndef VERIFICATIONITEM_H
#define VERIFICATIONITEM_H

#include <QWidget>
#include "tcpclient.h"

typedef struct _verify_info
{
    QString icon;//发送者头像
     int account;//目标账号
    QString name; //发送者名字
    QString msg;//验证信息
    int sender; //请求发送者
    QString groupName;//群组名
}VerifyInfo;

namespace Ui {
class VerificationItem;
}

class VerificationItem : public QWidget
{
    Q_OBJECT

public:
    explicit VerificationItem(VerifyInfo _info,TcpClient* _socket,int _type = 0, QWidget *parent = nullptr);
    ~VerificationItem();

    int getType(){return type;}

private slots:
    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_pushButton_clicked();

private:
    Ui::VerificationItem *ui;
    VerifyInfo info;
    TcpClient* socket;
    //0:好友请求 1:群组请求
    int type;
};

#endif // VERIFICATIONITEM_H
