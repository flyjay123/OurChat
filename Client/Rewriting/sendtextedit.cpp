#include <QDebug>
#include <QTextFragment>
#include <QTextDocumentFragment>
#include "sendtextedit.h"

QString extractPlainTextFromHtml(const QString &html) {
    QTextDocumentFragment fragment = QTextDocumentFragment::fromHtml(html);
    return fragment.toPlainText();
}

void SendTextEdit::keyPressEvent(QKeyEvent* event)
{
    //回车
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        if(event->modifiers() == Qt::ControlModifier)
        {
            append("");
        }
        else
            emit keyPressEnter();
    }
    //Ctrl+V
    else if((event->modifiers() & Qt::ControlModifier) && event->key() == Qt::Key_V)
    {
        const QMimeData *mimeData = QApplication::clipboard()->mimeData();
        if (mimeData->hasImage()) {
            // 从剪切板获取图片
            QImage image = qvariant_cast<QImage>(mimeData->imageData());

            // 将图片插入聊天框中
            // 将 QImage 转换为 QPixmap
            QPixmap pixmap = QPixmap::fromImage(image);

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

            // 获取 QTextEdit 的 QTextCursor
            QTextCursor cursor = textCursor();

            // 将图片插入 QTextEdit
            //cursor.insertImage(imageFormat);

            // 你也可以将图片插入到 QTextEdit 作为 HTML（可选）
             insertHtml("<img src='data:image/png;base64," + base64Image + "' />");
        }

        else if (mimeData->hasText()) {
            // 如果剪切板中有文本，执行正常的粘贴操作
            QTextEdit::keyPressEvent(event);
        }
    }
    else
     {
         QTextEdit::keyPressEvent(event);
     }
}

void SendTextEdit::mouseDoubleClickEvent(QMouseEvent* event)
{
    QTextCursor cursor = cursorForPosition(event->pos());
    QTextBlock block = cursor.block();
    QTextFragment fragment = block.begin().fragment();

    if (fragment.isValid()) {
        QTextImageFormat imageFormat = fragment.charFormat().toImageFormat();

        if (imageFormat.isValid()) {
            QString imagePath = imageFormat.name();

            // 在此处打开预览窗口
            // ...
            event->accept();
            return;
        }
    }

    QTextEdit::mouseDoubleClickEvent(event);
}


