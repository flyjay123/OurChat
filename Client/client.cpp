#include "client.h"
#include "ui_client.h"
#include <QGraphicsDropShadowEffect>
#include <QGuiApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <windows.h>
#include <QMouseEvent>
#include <QDebug>
#include "addfriend.h"
#include "frienditem.h"
#include <QListWidget>
#include <QToolTip>
#include "stringtool.h"
#include <QShortcut>
#include "sendtextedit.h"
#include "selfinfowidget.h"
#include "verificationitem.h"


Client::Client(SelfInfo info ,TcpClient* tcp,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);
    InitUI();
    t = tcp;
    selfInfo.name = info.name;
    selfInfo.account=info.account;
    selfInfo.password=info.password;
    messagesListWidget = new ChatListWidget(ItemType_Message);
    friendsListWidget = new ChatListWidget(ItemType_Friend);
    groupsListWidget = new ChatListWidget(ItemType_Group);


    connect(t,&TcpClient::CallClient,this,&Client::ClientMsgHandler);
    connect(ui->textEdit_send,&SendTextEdit::keyPressEnter,this,&Client::on_pushBtn_send_clicked);
    connect(friendsListWidget,&ChatListWidget::itemDoubleClicked,this,&Client::on_listWidget_info_itemClicked);
    connect(messagesListWidget,&ChatListWidget::itemDoubleClicked,this,[&](QListWidgetItem *item){
        FriendItem* friendItem = qobject_cast<FriendItem*>(messagesListWidget->itemWidget(item));
        SetChatWindow(friendItem);
    });
    connect(groupsListWidget,&ChatListWidget::itemDoubleClicked,this,&Client::on_groupsListWidget_itemClicked);

    ui->stackedWidget_list->addWidget(messagesListWidget);
    ui->stackedWidget_list->addWidget(friendsListWidget);
    ui->stackedWidget_list->addWidget(groupsListWidget);
    RefreshFriendList();

    VerifyInfo vinfo = {"",10001,"test","this is a test",selfInfo.account};
    VerificationItem *item = new VerificationItem(vinfo,t);
    item->show();
}

Client::~Client()
{
    delete ui;
}

void Client::RefreshFriendList()
{
    json msg;
    msg.insert("cmd",cmd_friend_list);
    msg.insert("account",QString("%1").arg(selfInfo.account));
    t->SendMsg(msg);
}

void Client::RefreshGroupList()
{
    json msg;
    msg.insert("cmd",cmd_group_list);
    msg.insert("account",QString("%1").arg(selfInfo.account));
    t->SendMsg(msg);
}

void Client::InitUI()
{
    this->setWindowTitle("OurChat");
    //    int width = this->width()-10;
    //    int height = this->height()-10;
    //    ui->centerWidget->setGeometry(5,5,width,height);
    //    ui->centerWidget->setStyleSheet("QWidget#centerWidget{ border-radius:4px; background:rgba(255,255,255,1); }");
    this->setWindowFlags(Qt::FramelessWindowHint);          //去掉标题栏无边框
    this->setAttribute(Qt::WA_TranslucentBackground,true);
    //实例阴影shadow
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    //设置阴影距离
    shadow->setOffset(0, 0);
    //设置阴影颜色
    shadow->setColor(QColor(39,40,43,100));
    //设置阴影圆角
    shadow->setBlurRadius(10);
    //给嵌套QWidget设置阴影
    ui->centerWidget->setGraphicsEffect(shadow);


    m_isFull = false;
}

void Client::mousePressEvent(QMouseEvent *event)
{
    mouseWindowTopLeft = event->pos();
}

void Client::mouseMoveEvent(QMouseEvent *event)
{
    //窗口移动
    if (event->buttons() & Qt::LeftButton)
    {
        mouseDeskTopLeft = event->globalPos();
        windowDeskTopLeft = mouseDeskTopLeft - mouseWindowTopLeft;  //矢量计算
        this->move(windowDeskTopLeft);     //移动到目的地
    }
}

void Client::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    if(m_isFull){
        //取消全屏
        m_isFull = false;
        ui->centerWidget->setGeometry(m_rect);

        ui->centerWidget->move(QApplication::desktop()->screen()->rect().center() - ui->centerWidget->rect().center());
    }
    else {
        m_isFull = true;
        m_rect = ui->centerWidget->rect();
        setGeometry(QGuiApplication::primaryScreen()->availableGeometry()); // 不包含windows任务栏区域
        ui->centerWidget->setGeometry(this->rect());
    }
}


void Client::on_pushBtn_max_clicked()
{
    //    this->showFullScreen(); //全屏
    if(m_isFull){
        //取消全屏
        m_isFull = false;
        ui->centerWidget->setGeometry(640,480,m_rect.width(),m_rect.height());
        ui->centerWidget->move(QApplication::desktop()->screen()->rect().center() - ui->centerWidget->rect().center());
    }
    else {
        m_isFull = true;
        m_rect = ui->centerWidget->rect();
        setGeometry(QGuiApplication::primaryScreen()->availableGeometry()); // 不包含windows任务栏区域
        ui->centerWidget->setGeometry(this->rect());
    }
    //    ui->centerWidget->showMaximized();
    //    this->showMaximized();
}

void Client::on_pushBtn_close_clicked()
{
    json msg ={ {"cmd",cmd_logout}};
    t->SendMsg(msg);
    this->close();
}

void Client::on_pushBtn_hide_clicked()
{
    QWidget* pWindow = this->window();
    if(pWindow->isTopLevel())
        pWindow->showMinimized();
}

void Client::on_pushButton_addFriend_clicked()
{
    AddFriend *add = new AddFriend(t);
    add->show();
}

void Client::on_pushBtn_refresh_clicked()
{
    switch(curListWidgetIndex) {
        case 0:return;
        case 1:RefreshFriendList();
        case 2:RefreshGroupList();
        default:
            return;
    }
}

void Client::ClientMsgHandler(json msg)
{
    int cmd = msg["cmd"].toInt();
    switch(cmd) {
        case cmd_friend_list: {
            friendsListWidget->clear();
            friendMap.clear();
            friendItemMap.clear();
            QJsonArray list = msg["msglist"].toArray();
            for (int i = 0; i < list.size(); i++) {
                FriendInfo info;
                json obj = list[i].toObject();
                info.name = obj["name"].toString();
                info.account = obj["account"].toString().toInt();
                info.sig = obj["sig"].toString();
                info.isOnline = obj["isOnline"].toBool();
                if (info.sig.isEmpty())
                    info.sig = "这家伙很高冷，啥也不想说";

                FriendItem *item = new FriendItem(info);

                QListWidgetItem *listItem = new QListWidgetItem(friendsListWidget);
                listItem->setSizeHint(QSize(260, 85));
                friendsListWidget->addItem(listItem);
                friendsListWidget->setItemWidget(listItem, item);

                friendMap.insert(item->account(), info);
                friendItemMap.insert(info.account, item);
            }
            break;
        }
        case cmd_friend_chat: {
            int account = msg["sender"].toString().toInt();
            ChatWindow *chatWindow;
            if (chatMap.find(account) == chatMap.end())  //账号对应的聊天窗口不存在
            {
                if (friendMap.find(account) != friendMap.end()) {
                    FriendInfo info = friendMap[account];
                    chatWindow = new ChatWindow(info);

                    ui->stackedWidget->addWidget(chatWindow);
                    chatMap.insert(account, chatWindow);
                } else {
                    //单向好友
                    return;
                }
            } else {
                chatWindow = chatMap.value(account);
                // ui->stackedWidget->setCurrentWidget(chatMap.value(account));
            }
            QString pushMsg = StringTool::MergePushMsg(currentDateTime, chatWindow->GetName(),
                                                       msg["sendmsg"].toString());
            chatWindow->pushMsg(pushMsg, 1);
            FriendItem *item = friendItemMap.value(account);
            if (account != curChatAccount) {
                item->NewMsgPlusOne();
            }
            else {
                if(curChatType == 1)
                    item->NewMsgPlusOne();
            }
            break;
        }
        case cmd_group_list:{
            groupsListWidget->clear();
            groupItemMap.clear();
            QJsonArray list = msg["msglist"].toArray();
            for (int i = 0; i < list.size(); i++) {
                GroupInfo info;
                json obj = list[i].toObject();
                info.groupName= obj["name"].toString();
                info.groupAccount = obj["account"].toString().toInt();

                FriendItem *item = new FriendItem(info);

                QListWidgetItem *listItem = new QListWidgetItem(groupsListWidget);
                listItem->setSizeHint(QSize(260, 85));
                groupsListWidget->addItem(listItem);
                groupsListWidget->setItemWidget(listItem, item);

                groupItemMap.insert(item->account(), item);
                groupMap.insert(info.groupAccount, info);
                json msg1 = {{"cmd",      cmd_group_member_list},
                             {"groupAccount", info.groupAccount}};
                t->SendMsg(msg1);
            }
            break;
        }
        case cmd_group_chat: {
            int sender = msg["sender"].toInt();
            int groupAccount = msg["groupAccount"].toInt();
            QString senderName;
            for(auto it:groupMap[groupAccount].memberList)
            {
                if(it.account == sender)
                {
                    senderName = it.name;
                    break;
                }
            }

            ChatWindow *chatWindow;
            if (groupChatMap.find(groupAccount) == groupChatMap.end())  //群聊账号对应的聊天窗口不存在
            {
                GroupInfo info = groupItemMap[groupAccount]->GetGroupInfo();
                chatWindow = new ChatWindow(info);

                ui->stackedWidget->addWidget(chatWindow);
                groupChatMap.insert(groupAccount, chatWindow);

            } else {
                chatWindow = groupChatMap.value(groupAccount);
                // ui->stackedWidget->setCurrentWidget(chatMap.value(account));
            }
            QString pushMsg = StringTool::MergePushMsg(currentDateTime, senderName,
                                                       msg["msg"].toString());
            chatWindow->pushMsg(pushMsg, 1);
            FriendItem *item = friendItemMap.value(groupAccount);
            if (groupAccount != curChatAccount) {
                item->NewMsgPlusOne();
            } else {
                if (curChatType == 0)
                    item->NewMsgPlusOne();
            }
            break;
        }
            case cmd_group_member_list:{
                int groupAccount = msg["groupAccount"].toInt();
                if(groupMap.find(groupAccount) == groupMap.end())
                    return;
                QJsonArray list = msg["msglist"].toArray();
                for (int i = 0; i < list.size(); i++) {
                    MemberInfo info;
                    json obj = list[i].toObject();
                    info.account = obj["account"].toString().toInt();
                    info.name = obj["name"].toString();
                    groupMap[groupAccount].memberList.push_back(info);
                }
                break;
            }

        default:
            break;
    }
}

void Client::on_listWidget_info_itemClicked(QListWidgetItem *item)
{

    FriendItem* friendItem = qobject_cast<FriendItem*>(friendsListWidget->itemWidget(item));
    SetChatWindow(friendItem);
    int account = friendItem->account();
    if(messageItemMap.find(account) == messageItemMap.end())
    {
        FriendItem *msgItem = new FriendItem(friendItem->GetFriendInfo());
        QListWidgetItem *listItem = new QListWidgetItem(messagesListWidget);
        listItem->setSizeHint(QSize(260, 85));
        friendsListWidget->addItem(listItem);
        friendsListWidget->setItemWidget(listItem, msgItem);

        messagesListWidget->addItem(listItem);
        messagesListWidget->setItemWidget(listItem, msgItem);

        messageItemMap.insert(account, msgItem);
    }
}
void Client::on_groupsListWidget_itemClicked(QListWidgetItem *item)
{
    FriendItem* friendItem = qobject_cast<FriendItem*>(groupsListWidget->itemWidget(item));
    SetChatWindow(friendItem);
    int account = friendItem->account();
    if(messageItemMap.find(account) == messageItemMap.end())
    {
        FriendItem *msgItem = new FriendItem(friendItem->GetGroupInfo());
        QListWidgetItem *listItem = new QListWidgetItem(messagesListWidget);
        listItem->setSizeHint(QSize(260, 85));
        groupsListWidget->addItem(listItem);
        groupsListWidget->setItemWidget(listItem, msgItem);

        messagesListWidget->addItem(listItem);
        messagesListWidget->setItemWidget(listItem, msgItem);

        messageItemMap.insert(account, msgItem);
    }
}

void Client::on_pushBtn_send_clicked()
{
    QString sendText = ui->textEdit_send->toPlainText();
    if( sendText== "")
    {
        QToolTip::showText(ui->pushBtn_send->mapToGlobal(QPoint(0, -50)), "发送的消息不能为空", ui->pushButton);
        return;
    }
    if(ui->stackedWidget->currentIndex() == 0)
    {
        QToolTip::showText(ui->pushBtn_send->mapToGlobal(QPoint(0, -50)), "选择的好友不能为空", ui->pushButton);
        return;
    }
    ChatWindow* chatWindow = qobject_cast<ChatWindow*>(ui->stackedWidget->currentWidget());
    int account = chatWindow->GetAccount();
    json msg={{"cmd",cmd_friend_chat},{"account",QString("%1").arg(account)},{"sendmsg",sendText},{"sender",QString("%1").arg(selfInfo.account)}};
    if(chatWindow->GetType())
    {
        msg["cmd"] = cmd_group_chat;
    }
    t->SendMsg(msg);
    ui->textEdit_send->clear();
    QString pushMsg = StringTool::MergePushMsg(currentDateTime,selfInfo.name,sendText);
    chatWindow->pushMsg(pushMsg,0);

}


void Client::on_pushBtn_refresh_2_clicked()
{
    t->waitForReadyRead(200);
}

void Client::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        on_pushBtn_send_clicked();
    }
}


void Client::on_pushButton_emoj_3_clicked()
{
    SelfInfoWidget* w = new SelfInfoWidget;
    w->show();
}

void Client::on_pushButton_msg_list_clicked()
{
    curListWidgetIndex = 0;
    ui->stackedWidget_list->setCurrentWidget(messagesListWidget);
}

void Client::on_pushButton_friend_list_clicked()
{
    curListWidgetIndex = 1;
    ui->stackedWidget_list->setCurrentWidget(friendsListWidget);
}

void Client::on_pushButton_group_list_clicked()
{
    curListWidgetIndex = 2;
    ui->stackedWidget_list->setCurrentWidget(groupsListWidget);
}

void Client::SetChatWindow(FriendItem *item) {
    if(item == nullptr)
        return;
    int account = item->account();
    //Friend ChatWindow
    if(item->GetType() == 0) {
        if (chatMap.find(account) == chatMap.end())  //账号对应的聊天窗口不存在
        {
            FriendInfo info{account, item->getLabelName()};
            ChatWindow *chatWindow = new ChatWindow(info);
            ui->stackedWidget->addWidget(chatWindow);
            ui->stackedWidget->setCurrentWidget(chatWindow);
            chatMap.insert(account, chatWindow);
        } else {
            ui->stackedWidget->setCurrentWidget(chatMap.value(account));
        }
        curChatType = 0;
    }
    else
    {
        if (groupChatMap.find(account) == groupChatMap.end())  //账号对应的聊天窗口不存在
        {
            GroupInfo info{account, item->getLabelName()};
            ChatWindow *chatWindow = new ChatWindow(info);
            ui->stackedWidget->addWidget(chatWindow);
            ui->stackedWidget->setCurrentWidget(chatWindow);
            groupChatMap.insert(account, chatWindow);
        } else {
            ui->stackedWidget->setCurrentWidget(groupChatMap.value(account));
        }
        curChatType = 1;
    }
    item->SetNewMsgNum(0);
    curChatAccount = account;
    ui->label_info->setText(item->GetChatName());
}
