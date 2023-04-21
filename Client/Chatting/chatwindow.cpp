#include "chatwindow.h"
#include "ui_chatwindow.h"
#include <QScrollBar>
#include <QBuffer>

ChatWindow::ChatWindow(FriendInfo info,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatWindow),
    m_info(info)
{
    ui->setupUi(this);
    m_type = 0;
    m_name = info.name;
    m_account = info.account;
    ui->textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->textEdit->setLineWrapMode(QTextBrowser::WidgetWidth);

    //ui->textEdit->setPlainText(QString("System :  you are chatting with %1\n").arg(m_info->name));
//    sendMessage("hello");
//    sendMessage("hello world");
//    sendImage("C:\\Users\\k\\Documents\\GitHub\\OurChat\\Client\\src\\QQIcon\\chatgpt-logo.png");
//    sendMessage("hello");
//    sendImage("C:\\Users\\k\\Documents\\GitHub\\OurChat\\Client\\src\\QQIcon\\icon15.jpg");
//    sendMessage("hello world");
//    sendFile("a.pdf","C:\\pdf\\a.pdf");
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
void ChatWindow::sendMessage(const QString &text, int flag) {
    //设置textBrowser光标到最后
    QTextCursor cursor = textBrowser->textCursor();
    cursor.movePosition(QTextCursor::End);
    textBrowser->setTextCursor(cursor);

    QString processedText = text;
    processedText.replace("\n", "<br>");

    QString alignment = flag == 1 ? "left" : "right";
    QString bubble = QString("<table style='background-color: #e0e0e0; border-radius: 10px; padding: 5px; margin: 5px; display: inline-table; text-align: %1;'><tr><td>%2</td></tr></table><br>")
            .arg(alignment, processedText);
    textBrowser->insertHtml(bubble);
    textBrowser->verticalScrollBar()->setValue(textBrowser->verticalScrollBar()->maximum()); // 自动滚动到底部
    qDebug() << "send message";
}


// 发送图片消息
void ChatWindow::sendImage(const QString &imagePath,int flag) {
    //设置textBrowser光标到最后
    QTextCursor cursor = textBrowser->textCursor();
    cursor.movePosition(QTextCursor::End);
    textBrowser->setTextCursor(cursor);

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
    QString alignment = flag == 1?"left":"right";
    QString imageHtml = QString("<img src='%1' width='%2' height='%3' style='float: %4;' /><br style='clear: both;'>")
            .arg(imagePath).arg(newWidth).arg(newHeight).arg(alignment);
    textBrowser->insertHtml(imageHtml);
    textBrowser->verticalScrollBar()->setValue(textBrowser->verticalScrollBar()->maximum()); // 自动滚动到底部
    qDebug() << "send image";
}

void ChatWindow::sendFile(const QString &fileName, const QString &filePath,int flag) {
    QString alignment = flag == 1?"left":"right";
    QString fileHtml = QString("<a href='%1' style='text-decoration: none; float: %2;'><div style='background-color: #e0e0e0; border-radius: 10px; padding: 5px; margin: 5px; display: inline-block;'>%3</div></a><br style='clear: both;'>")
            .arg(filePath,alignment,fileName);
    textBrowser->insertHtml(fileHtml);
    textBrowser->verticalScrollBar()->setValue(textBrowser->verticalScrollBar()->maximum()); // 自动滚动到底部

}

void ChatWindow::sendImage(const QImage &image, int flag) {
    if(image.isNull())
    {
        textBrowser->insertPlainText("Broken image!");
    }
    //设置textBrowser光标到最后
    QTextCursor cursor = textBrowser->textCursor();
    cursor.movePosition(QTextCursor::End);
    textBrowser->setTextCursor(cursor);
    int maxWidth = 100;
    int maxHeight = 100;

    // 计算适当的宽度和高度，以保持图片的纵横比
    int newWidth = qMin(image.width(), maxWidth);
    int newHeight = newWidth * image.height() / image.width();

    if (newHeight > maxHeight) {
        newHeight = maxHeight;
        newWidth = newHeight * image.width() / image.height();
    }

    // 缩放图像
    QImage scaledImage = image.scaled(newWidth, newHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // 将 QImage 转换为 QPixmap
    QPixmap pixmap = QPixmap::fromImage(scaledImage);

    // 创建一个 QTextImageFormat 对象
    QTextImageFormat imageFormat;

    // 将 QPixmap 转换为 Base64 编码的二进制数据
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");
    buffer.close();

    // 将 Base64 编码的二进制数据转换为 QString
    QString base64Image = QString::fromLatin1(byteArray.toBase64().data());

    // 设置 QTextImageFormat 属性
    imageFormat.setName("data:image/png;base64," + base64Image);

    // 你也可以将图片插入到 QTextEdit 作为 HTML（可选）
    ui->textEdit->insertHtml("<img src='data:image/png;base64," + base64Image + "' />");
    ui->textEdit->insertHtml("<br>"); // 添加一个换行符
}






































