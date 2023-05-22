#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QTextBrowser>
#include "tcpclient.h"

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWindow(const FriendInfo& info, QWidget *parent = nullptr);
    explicit ChatWindow(const GroupInfo& info, QWidget *parent = nullptr);
    ~ChatWindow();

    FriendInfo GetFriendInfo(){return m_info;}
    GroupInfo GetGroupInfo(){return m_groupInfo;}
    int GetAccount(){return m_account;}
    //flag:0 friend 1 group
    int GetType(){return m_type;}
    QString GetName(){return m_name;}
    void pushMsg(const QString& msg, int flag);

    //flag:0 self 1 other
    void sendMessage(const QString &text,int flag = 0);
    void sendImage(const QString &imagePath,int flag = 0);
    void sendImages(const QList<QString>& base64Images, int flag);
    void sendFile(const QString &fileName, const QString &filePath,int flag = 0);
    void sendContentFromInput(const QString &htmlContent, int flag);

private:
    Ui::ChatWindow *ui;
    int m_account;
    QString m_name;
    FriendInfo m_info;
    GroupInfo m_groupInfo;
    int m_type;


};

#endif // CHATWINDOW_H
