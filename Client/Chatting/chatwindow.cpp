#include "chatwindow.h"
#include "ui_chatwindow.h"
#include <QScrollBar>
#include <QBuffer>
#include <QImageReader>
#include "stringtool.h"

ChatWindow::ChatWindow(const FriendInfo& info,QWidget *parent) :
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
    QFont defaultFont = ui->textEdit->font();
    defaultFont.setPointSize(13); // 设置字体大小，根据您的需要调整大小
    ui->textEdit->setFont(defaultFont);
}

ChatWindow::ChatWindow(const GroupInfo& info, QWidget *parent):
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
void ChatWindow::pushMsg(const QString& msg, int flag)
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
    // 设置textBrowser光标到最后
    QTextCursor cursor = textBrowser->textCursor();
    cursor.movePosition(QTextCursor::End);
    textBrowser->setTextCursor(cursor);

    QString processedText = text.toHtmlEscaped();
    processedText.replace("\n", "<br>");

    QString alignment = flag == 1 ? "left" : "right";
    QString bubble = QString("<table style='background-color: #95EC69; margin: 10px; display: inline-table; text-align: %1; border-collapse: separate; border-spacing: 0;'>"
                             "<tr>"
                             "<td style='border-radius: 20px; padding: 6px;'>%2</td>"
                             "</tr>"
                             "</table>"
                             "<br>")
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

void ChatWindow::sendImages(const QList<QString>& base64Images, int flag) {
    for (const QString& base64Image : base64Images) {
        QByteArray imageData = QByteArray::fromBase64(base64Image.toLatin1());
        QImage image;
        image.loadFromData(imageData);
        // 保存原始图像的 Base64 编码
        QString originalBase64Image = "data:image/png;base64," + base64Image;
        // 创建缩略图
        int originalWidth = image.width();
        int originalHeight = image.height();
        int maxSize = 150;
        if (originalWidth > maxSize || originalHeight > maxSize) {
            // Only scale the image if either dimension is greater than 150 pixels
            float aspectRatio = static_cast<float>(originalWidth) / static_cast<float>(originalHeight);
            int newWidth, newHeight;

            if (aspectRatio >= 1) {
                // Width is greater than height
                newWidth = maxSize;
                newHeight = static_cast<int>((float)maxSize / aspectRatio);
            } else {
                // Height is greater than width
                newHeight = maxSize;
                newWidth = static_cast<int>((float)maxSize * aspectRatio);
            }

            image = image.scaled(newWidth, newHeight, Qt::KeepAspectRatio, Qt::FastTransformation);
        }
        QPixmap pixmap = QPixmap::fromImage(image);
        // 将缩略图插入 QTextEdit
        QTextImageFormat imageFormat;
        imageFormat.setWidth(pixmap.width());
        imageFormat.setHeight(pixmap.height());
        imageFormat.setName("data:image/png;base64," + base64Image);
        imageFormat.setProperty(QTextFormat::UserProperty, originalBase64Image);

        // 创建新的 QTextCursor
        QTextCursor cursor = textBrowser->textCursor();
        cursor.movePosition(QTextCursor::End);
        cursor.insertImage(imageFormat);

        // 插入换行
        pushMsg("",flag);

        // 设置文本对齐方式
        QTextBlockFormat blockFormat;
        blockFormat.setAlignment(flag ? Qt::AlignLeft : Qt::AlignRight);
        cursor.setBlockFormat(blockFormat);
    }

    textBrowser->verticalScrollBar()->setValue(textBrowser->verticalScrollBar()->maximum()); // 自动滚动到底部
    qDebug() << "send images";
}


void ChatWindow::sendContentFromInput(const QString& htmlContent, int flag) {
    // 设置textBrowser光标到最后
    QTextCursor cursor = textBrowser->textCursor();
    cursor.movePosition(QTextCursor::End);
    textBrowser->setTextCursor(cursor);

    QList<QPair<QString, QImage>> contents = StringTool::extractContent(htmlContent);

    for (const auto& content : contents) {
        if (!content.first.isEmpty()) {
            // 发送文字消息
            sendMessage(content.first, flag);
        }

        if (!content.second.isNull()) {
            // 发送图片消息
            QList<QString> base64Images;
            QByteArray ba;
            QBuffer buffer(&ba);
            buffer.open(QIODevice::WriteOnly);
            content.second.save(&buffer, "PNG");
            base64Images.append(ba.toBase64());
            sendImages(base64Images, flag);
        }
    }

    textBrowser->verticalScrollBar()->setValue(textBrowser->verticalScrollBar()->maximum()); // 自动滚动到底部
    qDebug() << "send content from input";
}








































