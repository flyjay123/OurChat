#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
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

class TcpClient :public QObject
{
    Q_OBJECT
public:
    TcpClient(QObject *parent = nullptr);
    ~TcpClient();
    int ConnetToServer();
    void DisconnetFormServer();

public:
    void SendMsg(json message);
    json GetMessage(){return  message;}
    //int CmdParser(json message);
    //QTcpSocket * GetSocket();
    bool WaitForSignal(unsigned int millisecond);


signals:
    // 收到消息信号
    void messageReceived();

public slots:
    void onReadyRead();

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
public:

#if 1
    QString m_server = "101.43.107.198";
#else
    QString m_server = "127.0.0.1";
#endif
    int m_port = 8888;
    bool m_isConnected;

    json message;

public:
   static QTcpSocket * g_socket;

};
extern TcpClient t;

#endif // TCPCLIENT_H
