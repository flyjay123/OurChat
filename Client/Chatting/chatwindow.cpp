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
    sendMessage("hello");
    sendMessage("hello world");
    sendImage("C:\\Users\\k\\Documents\\GitHub\\OurChat\\Client\\src\\QQIcon\\chatgpt-logo.png");
    sendMessage("hello");
    sendImage("C:\\Users\\k\\Documents\\GitHub\\OurChat\\Client\\src\\QQIcon\\icon15.jpg");
    sendMessage("hello world");
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
#define textBrowser ui->textEdit
// 发送文本消息
void ChatWindow::sendMessage(const QString &text) {
    QString bubble = QString("<table style='background-color: #e0e0e0; border-radius: 10px; padding: 5px; margin: 5px; display: inline-table;'><tr><td>%1</td></tr></table><br>").arg(text);
    textBrowser->insertHtml(bubble);
    textBrowser->insertPlainText("\n"); // 添加一个空行，确保每条消息都在新的一行
    textBrowser->verticalScrollBar()->setValue(textBrowser->verticalScrollBar()->maximum()); // 自动滚动到底部
}

// 发送图片消息
void ChatWindow::sendImage(const QString &imagePath) {
    QImage image(imagePath);
    int maxWidth = 100;
    int maxHeight = 100;

    // 计算适当的宽度和高度，以保持图片的纵横比
    int newWidth = qMin(image.width(), maxWidth);
    int newHeight = newWidth * image.height() / image.width();

    if (newHeight > maxHeight) {
        newHeight = maxHeight;
        newWidth = newHeight * image.width() / image.height();
    }

    QString imageHtml = QString("<img src='%1' width='%2' height='%3' /><br>").arg(imagePath).arg(newWidth).arg(newHeight);
    textBrowser->insertHtml(imageHtml);
    textBrowser->insertPlainText("\n"); // 添加一个空行，确保每条消息都在新的一行
    textBrowser->verticalScrollBar()->setValue(textBrowser->verticalScrollBar()->maximum()); // 自动滚动到底部
}

void ChatWindow::sendFile(const QString &fileName, const QString &filePath) {
    QString fileHtml = QString("<a href='%1' style='text-decoration: none;'><div style='background-color: #e0e0e0; border-radius: 10px; padding: 5px; margin: 5px; display: inline-block;'>%2</div></a><br>").arg(filePath).arg(fileName);
    textBrowser->insertHtml(fileHtml);
    textBrowser->insertPlainText("\n"); // 添加一个空行，确保每条消息都在新的一行
    textBrowser->verticalScrollBar()->setValue(textBrowser->verticalScrollBar()->maximum()); // 自动滚动到底部

}



