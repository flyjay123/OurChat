#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include  <QNetworkRequest >
#include <QNetworkReply>

using std::string;
using json = QJsonObject;
using jsonDoc = QJsonDocument;
using jsonArray = QJsonArray;

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
    cmd_group_join,
    cmd_group_list,
    cmd_group_chat,
    cmd_group_member_list,
    cmd_group_member_add,
    cmd_group_member_del,
};

typedef  struct _self_info
{
    int account;
    QString password;
    QString name;
    QString sig;
}SelfInfo ;

typedef struct _friend_info
{
    int account;
    QString name;
    QString sig;
    bool isOnline;
    _friend_info(int _account,QString _name){
        name=_name;account=_account;
    }
    _friend_info(){}
}FriendInfo;

class TcpClient : public QTcpSocket
{
    Q_OBJECT
public:
    TcpClient(QObject *parent = nullptr);
    ~TcpClient();
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
    QString m_server = "***REMOVED***";
#else
    QString m_server = "127.0.0.1";
#endif
    int m_port = 2339;
    bool m_isConnected = false;

    json message;
};
#endif // TCPCLIENT_H
