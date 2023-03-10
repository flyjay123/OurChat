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
    ConnetToServer();

    //connect(t.socket,&QTcpSocket::readyRead,this,&TcpClient::onReadyRead);
    connect(this,&QTcpSocket::readyRead,this,&TcpClient::onReadyRead);
}

TcpClient::~TcpClient()
{
    DisconnetFormServer();
}

int TcpClient::ConnetToServer()
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

void TcpClient::DisconnetFormServer()
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
    QString cmd = msg["cmd"].toString();
    if(cmd == "login" || cmd == "regist")
    {
        emit CallLogging(msg);
    }
    if(cmd=="friend-list" || cmd == "pchat")
    {
        emit CallClient(msg);
    }
    if(cmd == "searchresult")
    {
        emit CallAddFriend(msg);
    }
}






