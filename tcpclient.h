#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

using std::string;
using json = QJsonObject;
using jsonDoc = QJsonDocument;

typedef struct _login_info
{
    string cmd;
    int account;
    string password;
    string name;
} LoginInfo;

typedef struct _send_info
{
  string cmd;
  string info;
}SendInfo;

class TcpClient
{
public:
    TcpClient();
    ~TcpClient();
    int ConnetToServer();
    void DisconnetFormServer();

public:
    void SendMsg(json message);
    void RecvMsg();
    //int WatiRecvMsg(long ms);
    json GetMessage(){return  message;}
    //int CmdParser(json message);
    //QTcpSocket * GetSocket();
    void onReadyRead();
signals:
    // 收到消息信号
    void messageReceived(const QJsonObject &message);

private slots:
    // 连接成功槽
    void onConnected()
    {
        emit socket->connected();
    }


    // 断开连接槽
    void onDisconnected()
    {
        emit socket->disconnected();
    }
public:
    QTcpSocket * socket;
private:

    QString m_server = "101.43.107.198";
    //QString m_server = "127.0.0.1";
    int m_port = 8888;
    bool m_isConnected;

    json message;

public:
   static QTcpSocket * g_socket;

};
extern TcpClient t;

#endif // TCPCLIENT_H
