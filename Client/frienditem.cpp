#include "frienditem.h"
#include "ui_frienditem.h"

FriendItem::FriendItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendItem)
{
    ui->setupUi(this);
}

FriendItem::~FriendItem()
{
    delete ui;
}

void FriendItem::setName(const QString &name)
{
    ui->label_name->setText(name);
}

void FriendItem::setSignature(const QString &signature)
{
    ui->label_sig->setText(signature);
}
void FriendItem::setAvatar(const QPixmap &avatar)
{
    ui->label_icon->setPixmap(avatar);
}
