#include "tcpclient.h"
#include <QDebug>
#include <QTimer>

TcpClient::TcpClient()
{
    socket = new QTcpSocket();
    ConnetToServer();
}

TcpClient::~TcpClient()
{
    DisconnetFormServer();
}

int TcpClient::ConnetToServer()
{
    if(m_isConnected)
        return 1;
    socket->connectToHost(m_server,m_port);
    if(socket->waitForConnected(3000))
    {
         m_isConnected = true;
         return 0;
    }
    else
    {
        qDebug() << socket->errorString() << endl;
        return -1;
    }
}

void TcpClient::DisconnetFormServer()
{
    m_isConnected=false;
    socket->close();
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
    socket->write(lenBuf,4);
    // 发送数据到服务器
    socket->write(data);
    socket->waitForBytesWritten();
    qDebug() << "sendLen: " << lenBuf;
    qDebug() << "send: " << data;
}

void TcpClient::onReadyRead()
{
    char readLen[4];

    if(socket->waitForReadyRead(3000))
    {
        socket->read(readLen,4);
    }
    int len = atoi(readLen);
    qDebug() << "readlen: " << readLen;
    qDebug() << "len: " << len;


    QByteArray data = socket->read(len);
    QJsonDocument doc = QJsonDocument::fromJson(data);
    message = doc.object();
    qDebug() << "readsize: " << len;
    qDebug() << "read: " << data;
}




