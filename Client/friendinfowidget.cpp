#include "friendinfowidget.h"
#include "ui_friendinfowidget.h"

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
    ui->label_name->setText(info.name);
    ui->label_account->setNum(info.account);
    ui->textEdit_sig->setText(info.sig);
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
