#include "friendinfowidget.h"
#include "ui_friendinfowidget.h"
#include <QGraphicsDropShadowEffect>

FriendInfoWidget::FriendInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendInfoWidget)
{
    ui->setupUi(this);
}

FriendInfoWidget::FriendInfoWidget(FriendInfo info, QWidget *parent):
    QWidget(parent),ui(new Ui::FriendInfoWidget),m_info(info)
{
    ui->setupUi(this);
    InitUI();
    ui->label_name->setText(info.name);
    ui->label_account->setNum(info.account);
    ui->textEdit_sig->setText(info.sig);

}

FriendInfoWidget::FriendInfoWidget(GroupInfo info, QWidget *parent):
    QWidget(parent),ui(new Ui::FriendInfoWidget),m_groupInfo(info){
    ui->setupUi(this);
    InitUI();
    ui->label_name->setText(info.groupName);
    ui->label_account->setNum(info.groupAccount);
}

void FriendInfoWidget::enterEvent(QEvent *event)
{
    Q_UNUSED(event);emit enterWidget();
}

void FriendInfoWidget::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);leaveWidget();
}

FriendInfoWidget::~FriendInfoWidget()
{
    delete ui;
}

void FriendInfoWidget::InitUI() {

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
    ui->textEdit_sig->setFrameStyle(QFrame::NoFrame);
}
