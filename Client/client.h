#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QListWidgetItem>
#include <QMap>
#include <QTextEdit>
#include "tcpclient.h"
#include "chatwindow.h"
#include <QTime>
#include <QKeyEvent>
#include "frienditem.h"
#include "chatlistwidget.h"
#include "systemmessage.h"
#include "emojiselector.h"
#include <QSplitter>

extern bool friendWidgetOn;

QT_BEGIN_NAMESPACE
namespace Ui { class Client; }
QT_END_NAMESPACE



class Client : public QWidget
{
    Q_OBJECT

public:
    Client(SelfInfo info,TcpClient *t,QWidget *parent = nullptr);
    ~Client();

    void RefreshFriendList();
    void RefreshGroupList();
    void InitUI();
    void InitLayout();
    SelfInfo GetSelfInfo(){return selfInfo;}

    void SetChatWindow(FriendItem* item);
    ContentType CheckContentType(const QTextEdit *textEdit);

protected:
    // Event handlers
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event); //双击
    virtual void keyPressEvent(QKeyEvent *event);

public:
    QPoint mouseWindowTopLeft; //鼠标相对窗口左上角的坐标         在mousePressEvent 得到
    QPoint mouseDeskTopLeft;   //鼠标相对于桌面左上角坐标         在mouseMoveEvent实时获取
    QPoint windowDeskTopLeft;  //窗口左上角相对于桌面左上角坐标    在mouseMoveEvent实时计算(矢量)获得

private slots:
    void on_pushBtn_close_clicked();
    void on_pushBtn_hide_clicked();
    void on_pushBtn_max_clicked();
    void on_pushButton_emoj_3_clicked();

    void on_pushButton_msg_list_clicked();

    void on_pushButton_friend_list_clicked();

    void on_pushButton_group_list_clicked();

    void on_pushButton_system_msg_clicked();

    void on_pushButton_icon_clicked();

    void on_pushButton_emoj_clicked();

    void on_pushButton_image_clicked();

public slots:
    void on_pushButton_addFriend_clicked();
    void on_pushBtn_refresh_clicked();
    void on_listWidget_info_itemClicked(QListWidgetItem *item);
    void on_groupsListWidget_itemClicked(QListWidgetItem *item);

    void on_pushBtn_send_clicked();
    void insertEmoji(const QString &emoji);


public slots:
    void ClientMsgHandler(json msg);

signals:
    void iconChanged(QString url);

private:
    Ui::Client *ui;
    bool        m_isLogin;
    bool        m_isFull;
    QRect       m_rect;
    EmojiSelector* m_emojiSelector;
    SelfInfo selfInfo;
    int curChatAccount;
    //0:none 1:friend 2:group
    int curChatType = 0;
    //account, chatWindow
    QMap<int,ChatWindow*> chatMap;
    //group_account, group_chatWindow
    QMap<int,ChatWindow*> groupChatMap;
    //account, friendInfo
    QMap<int,FriendInfo> friendMap;
    //group_account, groupInfo
    QMap<int,GroupInfo> groupMap;
    //account,friendItem
    QMap<int,FriendItem*>friendItemMap;
    //group_account,groupItem
    QMap<int,FriendItem*>groupItemMap;
    //account,messageItem.prefix of account is f or g means friend or group
    QMap<QString,FriendItem*>messageItemMap;


    SystemMessage *systemMsg;
    QDateTime currentDateTime;
    TcpClient* t;

    ChatListWidget *messagesListWidget;
    ChatListWidget *friendsListWidget;
    ChatListWidget *groupsListWidget;
    int curListWidgetIndex;

    QSplitter *chatSplitter;
    QSplitter *mainSplitter;
};
#endif // CLIENT_H
