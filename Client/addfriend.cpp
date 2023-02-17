#include "addfriend.h"
#include "ui_addfriend.h"
#include <QGraphicsDropShadowEffect>

AddFriend::AddFriend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddFriend)
{
    ui->setupUi(this);
    Init();
}

AddFriend::~AddFriend()
{
    delete ui;
}

void AddFriend::Init()
{
    //this->setWindowFlags(Qt::FramelessWindowHint);          //去掉标题栏无边框
    //this->setAttribute(Qt::WA_TranslucentBackground,true);
    //实例阴影shadow
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    //设置阴影距离
    shadow->setOffset(0, 0);
    //设置阴影颜色
    shadow->setColor(QColor(39,40,43,100));
    //设置阴影圆角
    shadow->setBlurRadius(10);
    ui->textEdit->setPlaceholderText("验证信息");
    setWindowTitle("查找");
}
