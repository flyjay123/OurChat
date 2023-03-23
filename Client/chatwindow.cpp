#include "chatwindow.h"
#include "ui_chatwindow.h"
#include <QScrollBar>

ChatWindow::ChatWindow(FriendInfo info,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    m_info = info;
    m_type = 0;
    m_name = info.name;
    m_account = info.account;
    //ui->textEdit->setPlainText(QString("System :  you are chatting with %1\n").arg(m_info->name));
}

ChatWindow::ChatWindow(GroupInfo info, QWidget *parent):
        QWidget(parent),
        ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    m_groupInfo = info;
    m_type = 1;
    m_account = info.groupAccount;
    m_name = info.groupName;
    //ui->textEdit->setPlainText(QString("System :  you are chatting with %1\n").arg(m_info->name));
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

//flag:0 self 1 other
void ChatWindow::pushMsg(QString msg, int flag)
{
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);

    QTextBlockFormat blockFormat = cursor.blockFormat();

    QTextCharFormat blueFormat;
    if(flag == 0)   //自己的消息
    {
        blueFormat.setForeground(Qt::red);
        //ui->textEdit->setAlignment(Qt::AlignLeft);
        blockFormat.setAlignment(Qt::AlignRight);
    }
    else if(flag == 1)//别人的消息
    {
        blueFormat.setForeground(Qt::blue);
        blockFormat.setAlignment(Qt::AlignLeft);
    }

    cursor.setBlockFormat(blockFormat);
    cursor.insertText(msg,blueFormat);
    ui->textEdit->append("");
    QScrollBar*bar = ui->textEdit->verticalScrollBar();
    bar->setValue(bar->maximum());

}


