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
    explicit FriendItem(GroupInfo _info,QWidget *parent = nullptr);
    ~FriendItem();
    void Init();

    int account(){return m_account;}
    void setAccount(int a){m_account = a;}
    void setName(const QString &name);
    void setSignature(const QString &signature);
    void setAvatar(const QPixmap &avatar);
    void SetNewMsgNum(int num);
    void NewMsgPlusOne();
    void SetLastMsg(const QString& msg);
    void SetInfo(FriendInfo _info);
    FriendInfo GetFriendInfo(){return info;}
    void SetInfo(GroupInfo _info);
    GroupInfo GetGroupInfo(){return groupInfo;}

    QString getLabelName();
    QString GetChatName(){return m_name;}
    int GetType(){return m_type;}

private slots:
    void on_lineEdit_newMsg_textChanged(const QString &arg1);
    void showInfoWidget();
    void closeInfoWidget();

private:
    bool enterStatus = false;
    Ui::FriendItem *ui;
    FriendInfo info;
    GroupInfo groupInfo;
    int m_account;
    QString m_name;
    // 0 friend 1 group
    int m_type;
    FriendInfoWidget *widget;
    QTimer *timer;
    static FriendInfoWidget *curWidget;
};

#endif // FRIENTITEM_H
