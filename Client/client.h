#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Client; }
QT_END_NAMESPACE

typedef  struct _self_info
{
    int account;
    QString password;
    QString name;
}SelfInfo ;

class Client : public QWidget
{
    Q_OBJECT

public:
    Client(SelfInfo info,QWidget *parent = nullptr);
    ~Client();

    void RefreshFriendList();

    void InitUI();

protected:
    // Event handlers
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event); //双击
public:
    QPoint mouseWindowTopLeft; //鼠标相对窗口左上角的坐标         在mousePressEvent 得到
    QPoint mouseDeskTopLeft;   //鼠标相对于桌面左上角坐标         在mouseMoveEvent实时获取
    QPoint windowDeskTopLeft;  //窗口左上角相对于桌面左上角坐标    在mouseMoveEvent实时计算(矢量)获得

private slots:

    void on_pushBtn_close_clicked();

    void on_pushBtn_hide_clicked();

    void on_pushBtn_max_clicked();
    void on_pushButton_addFriend_clicked();

    void on_pushBtn_refresh_clicked();

private:
    Ui::Client *ui;
    bool        m_isLogin;
    bool        m_isfull;
    QRect       m_rect;
    SelfInfo selfInfo;
};
#endif // CLIENT_H
