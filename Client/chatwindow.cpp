#include "chatwindow.h"
#include "ui_chatwindow.h"
#include <QScrollBar>

ChatWindow::ChatWindow(FriendInfo info,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    m_info = new FriendInfo(info);
    ui->textEdit->setPlainText(QString("System :  you are chatting with %1\n").arg(m_info->name));
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

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
