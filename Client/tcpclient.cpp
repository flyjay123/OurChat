#include "tcpclient.h"
#include <QDebug>
#include <QTimer>
#include <QEventLoop>

void slot_test()
{
    qDebug() << "slot test" << endl;
}

TcpClient::TcpClient(QObject *parent) :QTcpSocket(parent)
{
    ConnectToServer();

    //connect(t.socket,&QTcpSocket::readyRead,this,&TcpClient::onReadyRead);
    connect(this,&QTcpSocket::readyRead,this,&TcpClient::onReadyRead);
}

TcpClient::~TcpClient()
{
    DisconnectFromServer();
}

int TcpClient::ConnectToServer()
{
    if(m_isConnected)
        return 1;
    this->connectToHost(m_server,m_port);
    if(this->waitForConnected(3000))
    {
        qDebug() << "连接成功" << endl;
         m_isConnected = true;
         return 0;
    }
    else
    {
        qDebug() << this->errorString() << endl;
        return -1;
    }
}

void TcpClient::DisconnectFromServer()
{
    m_isConnected=false;
}

void TcpClient::SendMsg(json message)
{
    if(!m_isConnected)
        return;

    // 将QJsonObject转换为QJsonDocument
        jsonDoc doc(message);
    // 将QJsonDocument转换为二进制数据
    QByteArray data = doc.toJson(QJsonDocument::Compact);
    //发送大小
    char lenBuf[4];
    sprintf(lenBuf,"%d",data.length());
    this->write(lenBuf,4);
    // 发送数据到服务器
    this->write(data);
    this->waitForBytesWritten(1000);
    qDebug() << "sendLen: " << lenBuf;
    qDebug() << "send: " << data;
}

void TcpClient::onReadyRead()
{
    qDebug() << "ready read " << endl;

    char readLen[4];
    this->read(readLen,4);
    int len = atoi(readLen);
    QByteArray data;
    while (data.length() < len) {
            QByteArray newData = this->read(len - data.length());
            if (newData.isEmpty()) {
                break; // 没有更多的数据可读取了
            }
            data += newData;
        }

    QJsonDocument doc = QJsonDocument::fromJson(data);
    message = doc.object();
    qDebug() << "readsize: " << len;
    qDebug() << "realReadSize: " << data.length();
    qDebug() << "read: " << data;
    CmdParser(message);

}
void TcpClient::CmdParser(json message)
{
    json msg(message);
    int cmd = msg["cmd"].toInt();
    qDebug() << "cmd = " << cmd;
    if(cmd == cmd_login || cmd == cmd_regist)
    {
        emit CallLogging(msg);
    }
    if(cmd >= cmd_friend_list && cmd <= cmd_group_chat)
    {
        qDebug() << "Call Client";
        emit CallClient(msg);
    }
    if(cmd == cmd_friend_search || cmd == cmd_group_search)
    {
        emit CallAddFriend(msg);
    }
}






