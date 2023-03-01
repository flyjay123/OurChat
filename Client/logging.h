#ifndef LOGGING_H
#define LOGGING_H

#include <QWidget>
#include "ui_logging.h"
#include <QMessageBox>
#include <QMouseEvent>
#include "tcpclient.h"

namespace Ui {
class Logging;
}

class Logging : public QWidget
{
    Q_OBJECT
private:
    void Init();

protected:
    // Event handlers
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

public:
    QPoint mouseWindowTopLeft; //鼠标相对窗口左上角的坐标         在mousePressEvent 得到
    QPoint mouseDeskTopLeft;   //鼠标相对于桌面左上角坐标         在mouseMoveEvent实时获取
    QPoint windowDeskTopLeft;  //窗口左上角相对于桌面左上角坐标    在mouseMoveEvent实时计算(矢量)获得
    void PageSwitch(int pageIndex);
    explicit Logging(QWidget *parent = nullptr);
    ~Logging();

private slots:
    void on_pushbtn_regist_clicked();

    void on_pushButton_login_clicked();

    void on_pushBtn_hide_clicked();

    void on_pushBtn_close_clicked();


    void on_pushBtn_close_2_clicked();

    void on_pushBtn_hide_2_clicked();

    void on_pushButton_regist_clicked();

    void on_pushButton_return_clicked();
    
    
    void on_pushButton_seePassword_clicked();

    void CmdHandler(json msg);

private:
    Ui::Logging *ui;
    TcpClient* t;
    QMessageBox* msgBox;
};

#endif // LOGGING_H
