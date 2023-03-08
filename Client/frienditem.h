#ifndef FRIENTITEM_H
#define FRIENTITEM_H

#include <QWidget>
#include "chatwindow.h"
#include "friendinfowidget.h"
#include <QTimer>

namespace Ui {
class FriendItem;
}

class FriendItem : public QWidget
{
    Q_OBJECT

public:
    explicit FriendItem(FriendInfo _info,QWidget *parent = nullptr);
    ~FriendItem();

    int account(){return m_account;}
    void setAccount(int a){m_account = a;}
    void setName(const QString &name);
    void setSignature(const QString &signature);
    void setAvatar(const QPixmap &avatar);
    void SetNewMsgNum(int num);
    void NewMsgPlusOne();
    void SetInfo(FriendInfo _info);

    QString getName();

private slots:
    void on_lineEdit_newMsg_textChanged(const QString &arg1);
    void showInfoWidget();
    void closeInfoWidget();

private:
    Ui::FriendItem *ui;
    FriendInfo info;
    int m_account;
    FriendInfoWidget *widget;
    QTimer *timer;
};

#endif // FRIENTITEM_H
