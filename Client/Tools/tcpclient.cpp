#include "tcpclient.h"
#include <QDebug>
#include <QTimer>
#include <QEventLoop>
#include <QtEndian>

void slot_test()
{
    qDebug() << "slot test" << endl;
}

TcpClient::TcpClient(QObject *parent) :QTcpSocket(parent)
{
    buffer.clear();
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
        qDebug() << "connect to server successfully" << endl;
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
    qDebug() << "disconnect from server";
    m_isConnected=false;
}

void TcpClient::SendMsg(json message)
{
    if(!m_isConnected)
        return;

    jsonDoc doc(message);
    QByteArray data = doc.toJson(QJsonDocument::Compact);

    int dataLength = data.length();
    char lenBuf[4];
    memcpy(lenBuf, &dataLength, 4); // 使用memcpy将整数转换为字节

    this->write(lenBuf, 4);
    this->write(data);
    this->waitForBytesWritten(1000);

    qDebug() << "sendLen: " << dataLength;
    //qDebug() << "send: " << data;
}


void TcpClient::onReadyRead()
{
    buffer.append(this->readAll()); // 将所有可用数据追加到缓冲区
    while (buffer.size() >= 4) // 检查缓冲区是否至少包含4个字节的消息长度
    {
        int len;
        memcpy(&len, buffer.constData(), 4); // 从缓冲区的前4个字节获取消息长度

        if (buffer.size() >= len + 4) // 检查缓冲区是否包含完整的消息
        {
            QByteArray data = buffer.mid(4, len); // 提取有效数据（不包括长度前缀）
            buffer.remove(0, len + 4); // 从缓冲区中删除已处理的数据

            QJsonDocument doc = QJsonDocument::fromJson(data);
            message = doc.object();
            qDebug() << "readsize: " << len;
            qDebug() << "realReadSize: " << data.length();
            //qDebug() << "read: " << data;
            CmdParser(message);
        }
        else
        {
            break; // 如果缓冲区中没有完整的消息，等待下一次数据到达
        }
    }
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
    if(cmd >= cmd_add_friend_request && cmd <= cmd_group_member_list)
    {
        emit CallClient(msg);
    }
    if(cmd == cmd_friend_search || cmd == cmd_group_search)
    {
        emit CallAddFriend(msg);
    }
}






