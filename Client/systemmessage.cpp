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
