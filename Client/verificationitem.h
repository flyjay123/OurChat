#ifndef VERIFICATIONITEM_H
#define VERIFICATIONITEM_H

#include <QWidget>
#include "tcpclient.h"

typedef struct _verify_info
{
    QString icon;
     int account;
    QString name;
    QString msg;
    int sender;
}VerifyInfo;

namespace Ui {
class VerificationItem;
}

class VerificationItem : public QWidget
{
    Q_OBJECT

public:
    explicit VerificationItem(VerifyInfo _info,TcpClient* _socket,QWidget *parent = nullptr);
    ~VerificationItem();

private slots:
    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_pushButton_clicked();

private:
    Ui::VerificationItem *ui;
    VerifyInfo info;
    TcpClient* socket;
};

#endif // VERIFICATIONITEM_H
