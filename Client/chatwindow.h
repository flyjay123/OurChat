#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>

namespace Ui {
class ChatWindow;
}

typedef struct _friend_info
{
    int account;
    QString name;
}FriendInfo;


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
