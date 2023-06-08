#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include  <QNetworkRequest>
#include <QNetworkReply>
#include <QVector>
#include <utility>

using std::string;
using json = QJsonObject;
using jsonDoc = QJsonDocument;
using jsonArray = QJsonArray;
using Qt::endl;

enum commands{
    cmd_regist = 0,
    cmd_login,
    cmd_logout,
    cmd_friend_search,
    cmd_add_friend_request,
    cmd_add_friend_response,
    cmd_friend_list,
    cmd_friend_chat,

    cmd_group_create,
    cmd_group_search,
    cmd_group_join_request,
    cmd_group_join_response,
    cmd_group_list,
    cmd_group_chat,
    cmd_group_member_list,
    cmd_group_member_add,
    cmd_group_member_del,
    cmd_set_icon
};

enum ContentType { TextOnly, ImageOnly, MixedContent };

typedef  struct self_info
{
    int account = 0;
    QString password;
    QString name;
    QString sig;
    QString icon;
}SelfInfo ;

typedef struct friend_info
{
    int account = 0;
    QString name;
    QString sig;
    bool isOnline = false;
    QString icon;
    friend_info(int _account,QString _name){
        name=std::move(_name);account=_account;
    }
    friend_info()= default;
}FriendInfo;

typedef struct member_info
{
    int account = 0;
    QString name;
}MemberInfo;

typedef struct group_info
{
    int groupAccount;
    QString groupName;
    QVector <MemberInfo> memberList;
}GroupInfo;

class TcpClient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = nullptr);
    ~TcpClient() override;
    int ConnectToServer();
    void DisconnectFromServer();



public:
    void SendMsg(json message);
    void CmdParser(json message);

Q_SIGNALS:
    // 收到消息信号
    void CallClient(json msg);
    void CallLogging(json msg);
    void CallAddFriend(json msg);

public slots:
    void onReadyRead();

public:

#if 1
    QString m_server = "xxx.xxx.xxx.xxx";
#else
    QString m_server = "127.0.0.1";
#endif
    int m_port = 2339;
    bool m_isConnected = false;

    json message;
private:
    QByteArray buffer;
};
#endif // TCPCLIENT_H
