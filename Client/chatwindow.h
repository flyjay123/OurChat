#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include "tcpclient.h"

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWindow(FriendInfo info, QWidget *parent = nullptr);
    ~ChatWindow();

    FriendInfo* GetFriendInfo(){return m_info;}
    void pushMsg(QString msg, int flag);

private:
    Ui::ChatWindow *ui;
    int m_account;
    FriendInfo* m_info;
};

#endif // CHATWINDOW_H
