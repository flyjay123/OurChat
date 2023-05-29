#include "client.h"
#include "ui_client.h"
#include <QGraphicsDropShadowEffect>
#include <QGuiApplication>
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
#include <QTextBlock>
#include <QFileDialog>
#include "sendtextedit.h"
#include "selfinfowidget.h"
#include "verificationitem.h"
#include "IconSetting/iconselect.h"


Client::Client(SelfInfo info ,TcpClient* tcp,QWidget *parent)
    : QWidget(parent), ui(new Ui::Client),selfInfo(info),t(tcp)
{
    ui->setupUi(this);
    InitUI();
    InitLayout();
    systemMsg = new SystemMessage();
    ui->label_icon->SetIcon(info.icon);
    messagesListWidget = new ChatListWidget(ItemType_Message);
    friendsListWidget = new ChatListWidget(ItemType_Friend);
    groupsListWidget = new ChatListWidget(ItemType_Group);
    m_emojiSelector = new EmojiSelector();
    m_emojiSelector->adjustSize();

    connect(m_emojiSelector, &EmojiSelector::emojiSelected, this, &Client::insertEmoji);
    connect(t,&TcpClient::CallClient,this,&Client::ClientMsgHandler);
    connect(ui->textEdit_send,&SendTextEdit::keyPressEnter,this,&Client::on_pushBtn_send_clicked);
    connect(friendsListWidget,&ChatListWidget::itemDoubleClicked,this,&Client::on_listWidget_info_itemClicked);
    connect(messagesListWidget,&ChatListWidget::itemDoubleClicked,this,[&](QListWidgetItem *item){
        FriendItem* friendItem = qobject_cast<FriendItem*>(messagesListWidget->itemWidget(item));
        SetChatWindow(friendItem);
    });
    connect(groupsListWidget,&ChatListWidget::itemDoubleClicked,this,&Client::on_groupsListWidget_itemClicked);
    ui->pushBtn_refresh->setVisible(false);
    ui->stackedWidget_list->addWidget(messagesListWidget);
    ui->stackedWidget_list->addWidget(friendsListWidget);
    ui->stackedWidget_list->addWidget(groupsListWidget);
    RefreshFriendList();
    RefreshGroupList();
}

Client::~Client()
{
    delete ui;
}

void Client::RefreshFriendList()
{
    json msg;
    msg.insert("cmd",cmd_friend_list);
    msg.insert("account",selfInfo.account);
    t->SendMsg(msg);
}

void Client::RefreshGroupList()
{
    json msg;
    msg.insert("cmd",cmd_group_list);
    msg.insert("account",selfInfo.account);
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

void Client::InitLayout()
{
    QString style = QString("QSplitter {border: 1px solid grey}");

    mainSplitter = new QSplitter(Qt::Horizontal);
    mainSplitter->setStyleSheet(style);
    mainSplitter->setHandleWidth(5);

    mainSplitter->addWidget(ui->widget_side);
    mainSplitter->addWidget(ui->widget_list);
    //mainSplitter->addWidget(ui->widget_chatting);

     ui->centerWidget->layout()->replaceWidget(ui->widget_main,mainSplitter);
     ui->widget_main->hide();

    chatSplitter = new QSplitter(Qt::Vertical);
    chatSplitter->setHandleWidth(5);
    mainSplitter->setStyleSheet(style);

    chatSplitter->addWidget(ui->widget_2);
    chatSplitter->addWidget(ui->widget_chatWindow);
    chatSplitter->addWidget(ui->widget_4);
    chatSplitter->addWidget(ui->widget_send);
    chatSplitter->addWidget(ui->widget_3);

    ui->centerWidget->layout()->replaceWidget(ui->widget_chatting,chatSplitter);
    ui->widget_chatting->hide();

    mainSplitter->addWidget(chatSplitter);

    mainSplitter->setSizes(QList<int>() << ui->widget_side->width() << ui->widget_list->width() << ui->widget_chatting->width());
    chatSplitter->setSizes(QList<int>() << ui->widget_2->height() << 400 << ui->widget_4->height() << 100 << ui->widget_3->height());
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
        mouseDeskTopLeft = event->globalPosition().toPoint();
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

        ui->centerWidget->move(QGuiApplication::primaryScreen()->geometry().center() - ui->centerWidget->rect().center());
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
        ui->centerWidget->move(QGuiApplication::primaryScreen()->geometry().center() - ui->centerWidget->rect().center());

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
    if(pWindow->isWindow())
        pWindow->showMinimized();
}

void Client::on_pushButton_addFriend_clicked()
{
    AddFriend *add = new AddFriend(selfInfo,t);
    add->show();
}

void Client::on_pushBtn_refresh_clicked()
{
    switch(curListWidgetIndex) {
        case 0:break;
    case 1:RefreshFriendList();break;
    case 2:RefreshGroupList();break;
        default:
            break;
    }
}

void Client::ClientMsgHandler(json msg)
{
    int cmd = msg["cmd"].toInt();
    switch(cmd) {
        case cmd_add_friend_request:
        {
            VerifyInfo vinfo;
            vinfo.account = selfInfo.account;
            vinfo.name = msg["name"].toString();
            vinfo.msg = msg["sendmsg"].toString();
            vinfo.sender = msg["sender"].toInt();
            vinfo.icon = msg["icon"].toString();
            VerificationItem *item = new VerificationItem(vinfo,t);
            systemMsg->AddItem(item);

            break;
        }
        case cmd_add_friend_response:
        {
            QString res = msg["res"].toString();
            if(res == "yes")
                RefreshFriendList();
            break;
        }
        case cmd_friend_list: {
            friendsListWidget->clear();
            friendMap.clear();
            friendItemMap.clear();
            QJsonArray list = msg["msglist"].toArray();
            for (int i = 0; i < list.size(); i++) {
                FriendInfo info;
                json obj = list[i].toObject();
                info.name = obj["name"].toString();
                info.account = obj["account"].toInt();
                info.sig = obj["sig"].toString();
                info.isOnline = obj["online"].toBool();
                info.icon = obj["icon"].toString();
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
            int account = msg["sender"].toInt();
            QString friendKey = QString("f_%1").arg(account);
            FriendItem *item = nullptr;
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

            if(messageItemMap.find(friendKey) == messageItemMap.end())//消息列表中没有该好友
            {
                FriendInfo info = friendMap[account];
                item = new FriendItem(info);
                QListWidgetItem *listItem = new QListWidgetItem(messagesListWidget);
                listItem->setSizeHint(QSize(260, 85));
                messagesListWidget->addItem(listItem);
                messagesListWidget->setItemWidget(listItem, item);
                messageItemMap.insert(friendKey, item);
            }
            else
            {
                item = messageItemMap.value(friendKey);
            }
            QString pushMsg = StringTool::MergeSendTimeMsg(currentDateTime, 1,chatWindow->GetName());
            chatWindow->pushMsg(pushMsg, 1);
            //push msg on chat window
            ContentType type = (ContentType)msg["type"].toInt();
            switch (type) {
                case ContentType::TextOnly: {
                    chatWindow->sendMessage(msg["content"].toString(), 1);
                    item->SetLastMsg(msg["content"].toString());
                    break;
                }
                case ContentType::ImageOnly: {
                    QString sendImage = msg["content"].toString();
                    chatWindow->sendImages(StringTool::GetImagesFromHtml(sendImage), 1);
                    item->SetLastMsg("[图片]");
                    break;
                }
                case ContentType::MixedContent: {
                    QString content = msg["content"].toString();
                    chatWindow->sendContentFromInput(content, 1);
                    item->SetLastMsg("[自定义消息]");
                    break;
                }
                default:
                    break;

            }
            if (account != curChatAccount)
                item->NewMsgPlusOne();
            else if(curChatType != 0)
                item->NewMsgPlusOne();
            break;
        }
        case cmd_group_join_request:
        {
            VerifyInfo vinfo;
            vinfo.name = msg["name"].toString();
            vinfo.msg = msg["msg"].toString();
            vinfo.sender = msg["sender"].toInt();
            vinfo.account = msg["groupAccount"].toInt();
            vinfo.groupName = msg["groupName"].toString();
            vinfo.icon = msg["icon"].toString();
            VerificationItem *item = new VerificationItem(vinfo,t,1);
            QString msg1 = "用户" + vinfo.name + "请求加入群" + vinfo.groupName + QString::number(vinfo.account);
            systemMsg->AddItem(item);

            break;
        }
        case cmd_group_join_response:
        {
            QString res = msg["res"].toString();
            QString info;
            if(res == "yes")
            {
                info = "群聊申请已通过";
                RefreshGroupList();
            }
            else
                info = "群聊申请被拒绝";
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
                info.groupAccount = obj["account"].toInt();

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
            int groupAccount = msg["account"].toInt();
            QString groupKey = QString("g_%1").arg(groupAccount);
            FriendItem *item;
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
            if(messageItemMap.find(groupKey) == messageItemMap.end())//消息列表中没有该群聊
            {
                GroupInfo info = groupMap[groupAccount];
                item = new FriendItem(info);
                QListWidgetItem *listItem = new QListWidgetItem(messagesListWidget);
                listItem->setSizeHint(QSize(260, 85));
                messagesListWidget->addItem(listItem);
                messagesListWidget->setItemWidget(listItem, item);
                messageItemMap.insert(groupKey, item);
            }
            else
            {
                item = messageItemMap.value(groupKey);
            }
            ContentType type = (ContentType)msg["type"].toInt();
            switch (type) {
                case ContentType::TextOnly: {
                    QString content = msg["content"].toString();
                    QString pushMsg = StringTool::MergeSendTimeMsg(currentDateTime, 1,senderName);
                    chatWindow->pushMsg(pushMsg, 1);
                    chatWindow->sendMessage(content, 1);
                    item->SetLastMsg(content);
                    break;
                }
                case ContentType::ImageOnly: {
                    QString sendImage = msg["content"].toString();
                    QString pushMsg = StringTool::MergeSendTimeMsg(currentDateTime, 1,senderName);
                    chatWindow->pushMsg(pushMsg, 1);
                    chatWindow->sendImages(StringTool::GetImagesFromHtml(sendImage), 0);
                    item->SetLastMsg("[图片]");
                    break;
                }
                case ContentType::MixedContent: {
                    QString content = msg["content"].toString();
                    //QList<QPair<QString, QImage>> contentList = StringTool::extractContent(content);
                    QString pushMsg = StringTool::MergeSendTimeMsg(currentDateTime, 1,senderName);
                    chatWindow->pushMsg(pushMsg, 1);
                    //chatWindow->sendMixedContent(contentList, 1);
                    chatWindow->sendContentFromInput(content, 1);
                    item->SetLastMsg("[自定义消息]");
                    break;
                }
                default:
                    break;
            }
            if (groupAccount != curChatAccount) {
                item->NewMsgPlusOne();
            } else {
                if (curChatType != 1)
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
    QString friendKey = QString("f_%1").arg(account);
    if(messageItemMap.find(friendKey) == messageItemMap.end())
    {
        FriendItem *msgItem = new FriendItem(friendItem->GetFriendInfo());
        QListWidgetItem *listItem = new QListWidgetItem(messagesListWidget);
        listItem->setSizeHint(QSize(260, 85));
        friendsListWidget->addItem(listItem);
        friendsListWidget->setItemWidget(listItem, msgItem);

        messagesListWidget->addItem(listItem);
        messagesListWidget->setItemWidget(listItem, msgItem);

        messageItemMap.insert(friendKey, msgItem);
    }
}
void Client::on_groupsListWidget_itemClicked(QListWidgetItem *item)
{
    FriendItem* friendItem = qobject_cast<FriendItem*>(groupsListWidget->itemWidget(item));
    SetChatWindow(friendItem);
    int account = friendItem->account();
    QString groupKey = QString("g_%1").arg(account);
    if(messageItemMap.find(groupKey) == messageItemMap.end())
    {
        FriendItem *msgItem = new FriendItem(friendItem->GetGroupInfo());
        QListWidgetItem *listItem = new QListWidgetItem(messagesListWidget);
        listItem->setSizeHint(QSize(260, 85));
        groupsListWidget->addItem(listItem);
        groupsListWidget->setItemWidget(listItem, msgItem);

        messagesListWidget->addItem(listItem);
        messagesListWidget->setItemWidget(listItem, msgItem);

        messageItemMap.insert(groupKey, msgItem);
    }
}

void Client::on_pushBtn_send_clicked()
{
    ContentType type = CheckContentType(ui->textEdit_send);
    if(ui->stackedWidget->currentIndex() == 0)
    {
        QToolTip::showText(ui->pushBtn_send->mapToGlobal(QPoint(0, -50)), "选择的好友不能为空", ui->pushButton);
        return;
    }

    ChatWindow* chatWindow = qobject_cast<ChatWindow*>(ui->stackedWidget->currentWidget());
    int account = chatWindow->GetAccount();

    QString pushMsg = StringTool::MergeSendTimeMsg(currentDateTime, 0);
    json msg = {
            {"cmd", chatWindow->GetType() ? cmd_group_chat : cmd_friend_chat},
            {"account", account},
            {"sender", selfInfo.account},
            {"type", type},
            {"timestamp", currentDateTime.toString("yyyy-MM-dd hh:mm:ss")}
    };
    QString friendKey;
    if(curChatType == 1)
    {
        friendKey = QString("f_%1").arg(curChatAccount);
    }
    else if (curChatType == 2)
    {
        friendKey = QString("g_%1").arg(curChatAccount);
    }
    FriendItem *item = messageItemMap.value(friendKey);

    switch (type) {
        case TextOnly: {
            QString sendText = ui->textEdit_send->toPlainText();
            if(sendText.isEmpty())
            {
                QToolTip::showText(ui->pushBtn_send->mapToGlobal(QPoint(0, -50)), "发送的消息不能为空", ui->pushButton);
                return;
            }
            msg["content"] = (sendText );
            chatWindow->pushMsg(pushMsg, 0);
            chatWindow->sendMessage(msg["content"].toString(), 0);
            item->SetLastMsg(sendText);
            break;
        }
        case ImageOnly:
        {
            QString sendImage = ui->textEdit_send->toHtml();
            msg["content"] = sendImage;
            chatWindow->pushMsg(pushMsg, 0);
            chatWindow->sendImages(StringTool::GetImagesFromHtml(sendImage), 0);
            item->SetLastMsg("[图片]");
            break;
        }
        case MixedContent: {
            QString html = ui->textEdit_send->toHtml();
            //QList<QPair<QString, QImage>> contentList = StringTool::extractContent(html);
            //chatWindow->sendMixedContent(contentList,0);
            chatWindow->pushMsg(pushMsg, 0);
            chatWindow->sendContentFromInput(html, 0);
            msg["content"] = html;
            item->SetLastMsg("[自定义消息]");
            break;
        }

    }
    ui->textEdit_send->clear();
    //chatWindow->sendMessage(msg["content"].toString(), 0);
    t->SendMsg(msg);
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
    if(curListWidgetIndex == 0)
    {
        ui->pushButton_msg_list->setChecked(true);
        return;
    }
    else
    {
        ui->pushButton_friend_list->setChecked(false);
        ui->pushButton_group_list->setChecked(false);
    }
    curListWidgetIndex = 0;
    ui->stackedWidget_list->setCurrentWidget(messagesListWidget);
}

void Client::on_pushButton_friend_list_clicked()
{
    if(curListWidgetIndex == 1)
    {
        ui->pushButton_friend_list->setChecked(true);
        return;
    }
    else
    {
        ui->pushButton_msg_list->setChecked(false);
        ui->pushButton_group_list->setChecked(false);
    }
    curListWidgetIndex = 1;
    ui->stackedWidget_list->setCurrentWidget(friendsListWidget);
}

void Client::on_pushButton_group_list_clicked()
{
    if(curListWidgetIndex == 2)
    {
        ui->pushButton_group_list->setChecked(true);
        return;
    }
    else
    {
        ui->pushButton_msg_list->setChecked(false);
        ui->pushButton_friend_list->setChecked(false);
    }
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
        curChatType = 1;
    }
    else
    {
        if (groupChatMap.find(account) == groupChatMap.end())  //账号对应的聊天窗口不存在
        {
            GroupInfo info{account, item->getLabelName(),{}};
            ChatWindow *chatWindow = new ChatWindow(info);
            ui->stackedWidget->addWidget(chatWindow);
            ui->stackedWidget->setCurrentWidget(chatWindow);
            groupChatMap.insert(account, chatWindow);
        } else {
            ui->stackedWidget->setCurrentWidget(groupChatMap.value(account));
        }
        curChatType = 2;
    }
    item->SetNewMsgNum(0);
    curChatAccount = account;
    ui->label_info->setText(item->GetChatName());
}

void Client::on_pushButton_system_msg_clicked()
{
    if(systemMsg->isHidden())
        systemMsg->show();
}

void Client::on_pushButton_icon_clicked()
{
    IconSelect *iconSelect = new IconSelect;
    iconSelect->show();

    connect(iconSelect, &IconSelect::SetIcon, this, [&](QString url){
        ui->label_icon->SetIcon(url);
        selfInfo.icon = url;
        json msg = {{"cmd",cmd_set_icon},{"account",selfInfo.account},{"icon",url}};
        t->SendMsg(msg);
    });
}

ContentType Client::CheckContentType(const QTextEdit *textEdit)
{
    bool hasText = false;
    bool hasImage = false;

    QTextDocument *doc = textEdit->document();
    for (QTextBlock block = doc->begin(); block != doc->end(); block = block.next()) {
        for (QTextBlock::iterator it = block.begin(); !it.atEnd(); ++it) {
            QTextFragment fragment = it.fragment();
            if (fragment.isValid()) {
                if (fragment.charFormat().isImageFormat()) {
                    hasImage = true;
                } else {
                    hasText = true;
                }

                if (hasText && hasImage) {
                    return MixedContent;
                }
            }
        }
    }
    if (hasText) {
        return TextOnly;
    } else if (hasImage) {
        return ImageOnly;
    } else {
        return TextOnly; // 如果输入框为空，我们可以认为它是文本类型
    }
}

void Client::on_pushButton_emoj_clicked()
{
        QPoint buttonPos = ui->pushButton_emoj->mapToGlobal(QPoint(0, 0));
        int x = buttonPos.x() + (ui->pushButton_emoj->width() / 2) - (m_emojiSelector->width() / 2);
        int y = buttonPos.y() - m_emojiSelector->height();
        m_emojiSelector->move(x, y-10);
        m_emojiSelector->show();
}

void Client::insertEmoji(const QString &emoji)
{
    ui->textEdit_send->insertPlainText(emoji);
}



void Client::on_pushButton_image_clicked()
{
    //open file select dialog
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("Images (*.png *.jpg)"));
    if(fileName.isEmpty())
        return;
    QImage image(fileName);
    if(image.isNull())
        return;

    // Convert image to QPixmap
    QPixmap pixmap = QPixmap::fromImage(image);

    // Save QPixmap as PNG and convert it to Base64
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG"); // write pixmap into bytes in PNG format
    buffer.close();
    QString base64Image = byteArray.toBase64().data();

    // Insert image into QTextEdit as HTML
    ui->textEdit_send->insertHtml("<img src='data:image/png;base64," + base64Image + "' />");
    ui->textEdit_send->setFocus();
}

