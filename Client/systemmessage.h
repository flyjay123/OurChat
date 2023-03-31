#ifndef SYSTEMMESSAGE_H
#define SYSTEMMESSAGE_H

#include <QWidget>
#include <QListWidget>
#include <verificationitem.h>

namespace Ui {
class SystemMessage;
}

class SystemMessage : public QWidget
{
    Q_OBJECT

public:
    explicit SystemMessage(QWidget *parent = nullptr);
    ~SystemMessage();

    //type: 0:好友请求 1:群组请求
    void AddItem(VerificationItem *item);

private:
    Ui::SystemMessage *ui;
};

#endif // SYSTEMMESSAGE_H
