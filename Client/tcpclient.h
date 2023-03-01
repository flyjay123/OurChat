#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QMessageBox>

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

class TcpClient : public QTcpSocket
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
    void CmdParser(json message);

Q_SIGNALS:
    // 收到消息信号
    void messageReceived();
    void CallClient(json msg);
    void CallLogging(json msg);

public slots:
    void onReadyRead();

public:

#if 1
    QString m_server = "***REMOVED***";
#else
    QString m_server = "127.0.0.1";
#endif
    int m_port = 8888;
    bool m_isConnected = false;

    json message;
};
#endif // TCPCLIENT_H
