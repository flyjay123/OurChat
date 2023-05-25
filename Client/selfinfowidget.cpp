#include "selfinfowidget.h"
#include "ui_selfinfowidget.h"
#include <QGraphicsDropShadowEffect>

SelfInfoWidget::SelfInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelfInfoWidget)
{
    ui->setupUi(this);
    setStyleSheet("QWidget{border-radius:4px;background:rgba(255,255,255,1);}");  //设置圆角

    this->setWindowFlags(Qt::FramelessWindowHint);          //去掉标题栏无边框
    this->setAttribute(Qt::WA_TranslucentBackground,true);
    //实例阴影shadow
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    //设置阴影距离
    shadow->setOffset(0, 0);
    //设置阴影颜色
    shadow->setColor(QColor(39,40,43,100));
    //设置阴影圆角
    shadow->setBlurRadius(10);
    //给嵌套QWidget设置阴影
    setGraphicsEffect(shadow);

}

SelfInfoWidget::~SelfInfoWidget()
{
    delete ui;
}

void SelfInfoWidget::mousePressEvent(QMouseEvent *event)
{
    mouseWindowTopLeft = event->pos();
}

void SelfInfoWidget::mouseMoveEvent(QMouseEvent *event)
{
        //窗口移动
        if (event->buttons() & Qt::LeftButton)
        {
            mouseDeskTopLeft = event->globalPosition().toPoint();
            windowDeskTopLeft = mouseDeskTopLeft - mouseWindowTopLeft;  //矢量计算
            this->move(windowDeskTopLeft);     //移动到目的地
        }
}

void SelfInfoWidget::on_pushBtn_close_clicked()
{
    this->close();
}

void SelfInfoWidget::on_pushBtn_hide_clicked()
{
    QWidget* pWindow = this->window();
    if(pWindow->isWindow())
        pWindow->showMinimized();
}
