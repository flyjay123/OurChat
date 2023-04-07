#include "systemmessage.h"
#include "ui_systemmessage.h"

SystemMessage::SystemMessage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SystemMessage)
{
    ui->setupUi(this);
}

SystemMessage::~SystemMessage()
{
    delete ui;
}

void SystemMessage::AddItem(VerificationItem *item)
{
    QListWidget* widget;
    int type = item->getType();
    if(type == 0)
    {
        widget = ui->listWidget;
    }
    else
    {
        widget = ui->listWidget_2;
    }
    QListWidgetItem *listItem = new QListWidgetItem(widget);
    listItem->setSizeHint(QSize(0, 100));
    widget->addItem(listItem);
    widget->setItemWidget(listItem, item);
}
