#ifndef SELFINFOWIDGET_H
#define SELFINFOWIDGET_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class SelfInfoWidget;
}

class SelfInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SelfInfoWidget(QWidget *parent = nullptr);
    ~SelfInfoWidget();

protected:
    // Event handlers
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

public:
    QPoint mouseWindowTopLeft; //鼠标相对窗口左上角的坐标         在mousePressEvent 得到
    QPoint mouseDeskTopLeft;   //鼠标相对于桌面左上角坐标         在mouseMoveEvent实时获取
    QPoint windowDeskTopLeft;  //窗口左上角相对于桌面左上角坐标    在mouseMoveEvent实时计算(矢量)获得

private slots:
    void on_pushBtn_close_clicked();

    void on_pushBtn_hide_clicked();

private:
    Ui::SelfInfoWidget *ui;
};

#endif // SELFINFOWIDGET_H
