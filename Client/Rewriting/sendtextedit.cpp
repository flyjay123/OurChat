#include <QDebug>
#include <QTextFragment>
#include <QTextDocumentFragment>
#include "sendtextedit.h"
#include "imagepreview.h"

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
            imageFormat.setProperty(QTextFormat::UserProperty, "data:image/png;base64," + base64Image); // 保存原始图像的 Base64 编码


            // 获取 QTextEdit 的 QTextCursor
            QTextCursor cursor = textCursor();

            // 将图片插入 QTextEdit
            cursor.insertImage(imageFormat);

            // 你也可以将图片插入到 QTextEdit 作为 HTML（可选）
             //insertHtml("<img src='data:image/png;base64," + base64Image + "' />");
        }

        else if (mimeData->hasText()) {
            // 如果剪切板中有文本，执行正常的粘贴操作
            QTextEdit::keyPressEvent(event);
        }
    }
    //Ctrl+C
    else if ((event->modifiers() & Qt::ControlModifier) && event->key() == Qt::Key_C)
    {
        QTextEdit::keyPressEvent(event);
        // 获取剪切板对象
        QClipboard *clipboard = QApplication::clipboard();

        // 获取选中的图片
        QTextCursor cursor = textCursor();
        QTextBlock block = cursor.block();
        QTextFragment fragment = block.begin().fragment();
        if (fragment.isValid()) {
            QTextImageFormat imageFormat = fragment.charFormat().toImageFormat();
            if (imageFormat.isValid()) {
                QString originalImageData = imageFormat.property(QTextFormat::UserProperty).toString();
                QPixmap pixmap;
                QByteArray imageData;
                if(!originalImageData.isEmpty()) {

                    int base64Index = originalImageData.indexOf("base64,");
                    if (base64Index > 0) {
                        originalImageData = originalImageData.mid(base64Index + 7);
                    }
                    imageData = QByteArray::fromBase64(originalImageData.toLatin1());
                }
                else
                {
                    QString imagePath = imageFormat.name();
                    int base64Index = imagePath.indexOf("base64,");
                    if (base64Index > 0) {
                        imagePath = imagePath.mid(base64Index + 7);
                    }
                    pixmap.load(imagePath);
                    QBuffer buffer(&imageData);
                    buffer.open(QIODevice::WriteOnly);
                    pixmap.save(&buffer, "PNG");
                    buffer.close();
                }
                QImage image = QImage::fromData(imageData, "PNG");
                clipboard->setImage(image);
            }
        }

    }
    else
    {
        // 调用父类的方法
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
            QString originalImageData = imageFormat.property(QTextFormat::UserProperty).toString();
            QPixmap pixmap;
            QByteArray imageData;
            if(!originalImageData.isEmpty()) {

                int base64Index = originalImageData.indexOf("base64,");
                if (base64Index > 0) {
                    originalImageData = originalImageData.mid(base64Index + 7);
                }
                imageData = QByteArray::fromBase64(originalImageData.toLatin1());
            }
            else
            {
                QString imagePath = imageFormat.name();
                int base64Index = imagePath.indexOf("base64,");
                if (base64Index > 0) {
                    imagePath = imagePath.mid(base64Index + 7);
                }
                imageData = QByteArray::fromBase64(imagePath.toLatin1());
            }
            bool loaded = pixmap.loadFromData(imageData);
            if (!loaded || pixmap.isNull()) {
                qDebug() << "Failed to load image from data";
            } else {
                // 在此处打开预览窗口
                ImagePreview *preview = new ImagePreview(pixmap);
                preview->setWindowTitle("Image Preview");
                preview->setAttribute(Qt::WA_DeleteOnClose); // 窗口关闭时自动删除对象
                preview->show();
            }
            event->accept();
            return;
        }
    }
    QTextEdit::mouseDoubleClickEvent(event);
}




