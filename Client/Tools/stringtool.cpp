#include "stringtool.h"

StringTool::StringTool()
{

}

QString StringTool::Str2QStr(std::string str)
{
    QString qs = QString::fromStdString(str);
    return qs;
}

QString StringTool::MergePushMsg(QDateTime dateTime, QString name, QString text)
{
    dateTime = QDateTime::currentDateTime();
    QString dateTimeString = dateTime.toString("yyyy-MM-dd hh:mm:ss");
    QString pushMsg(dateTimeString+"\t"+name+": " + "\n" + text + "\n");
    return pushMsg;
}

QString StringTool::Int2QStr(int num)
{
    return QString("%1").arg(num);
}

QString StringTool::MergeSendTimeMsg(QDateTime dateTime, int flag, QString name) {
    dateTime = QDateTime::currentDateTime();
    QString dateTimeString = dateTime.toString("yyyy-MM-dd hh:mm:ss");
    if(flag)
    {
        return dateTimeString + "  " + name + ":";
    }
    else
    {
        return dateTimeString;
    }
}

QImage StringTool::GetImageFromHtml(const QString& html) {
    QRegularExpression re(R"(<img[^>]*src=[\"']data:image/(png|jpg|jpeg);base64,([^"]*)[\"'][^>]*>)", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = re.match(html);
    if (match.hasMatch()) {
        QString base64 = match.captured(2);
        QByteArray ba = QByteArray::fromBase64(base64.toLatin1());
        QImage image;
        image.loadFromData(ba);
        return image;
    } else {
        qDebug() << "no match";
        //qDebug() << html;
        return {};
    }
}

QList<QImage> StringTool::GetImagesFromHtml(const QString& html) {
    QList<QImage> images;
    QRegularExpression re("<img[^>]+src\\s*=\\s*['\"]data:image/(png|jpg|jpeg);base64,([^'\"]+)['\"][^>]*>", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatchIterator i = re.globalMatch(html);
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        QString base64 = match.captured(2);
        QByteArray ba = QByteArray::fromBase64(base64.toLatin1());
        QImage image;
        image.loadFromData(ba);
        images.append(image);
    }
    return images;
}

QList<QVariant> StringTool::GetContentsFromHtml(const QString& html) {
    QList<QVariant> contents;
    QRegularExpression re("<(/?[^>]+)>"); // 匹配 HTML 标签
    QRegularExpressionMatchIterator i = re.globalMatch(html);
    int lastPos = 0;
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        int start = match.capturedStart();
        int end = match.capturedEnd();
        if (start > lastPos) {
            // 添加纯文本内容
            contents.append(html.mid(lastPos, start - lastPos));
        }
        lastPos = end;

        // 检查是否为图片标签
        QString tag = match.captured(1).toLower();
        if (tag.startsWith("img")) {
            // 提取 Base64 编码的图片数据
            QRegularExpression reSrc("src\\s*=\\s*['\"]data:image/(png|jpg|jpeg);base64,([^'\"]+)['\"]", QRegularExpression::CaseInsensitiveOption);
            QRegularExpressionMatch srcMatch = reSrc.match(match.captured(0));
            if (srcMatch.hasMatch()) {
                QString base64 = srcMatch.captured(2);
                QByteArray ba = QByteArray::fromBase64(base64.toLatin1());
                QImage image;
                image.loadFromData(ba);
                contents.append(image);
            }
        }
    }
    if (lastPos < html.length()) {
        // 添加剩余的纯文本内容
        contents.append(html.mid(lastPos));
    }
    return contents;
}

QList<QPair<QString, QImage>> StringTool::extractContent(const QString& html) {
    QList<QPair<QString, QImage>> contentList;

    QXmlStreamReader reader(html);
    QString currentText;
    QImage currentImage;

    while (!reader.atEnd()) {
        reader.readNext();

        if (reader.isStartElement()) {
            if (reader.name().toString() == "p") {
                currentText.clear();
            } else if (reader.name().toString() == "img") {
                QStringView base64 = reader.attributes().value("src").mid(22);
                QByteArray ba = QByteArray::fromBase64(base64.toLatin1());
                currentImage.loadFromData(ba);
            }
        } else if (reader.isEndElement()) {
            if (reader.name().toString() == "p") {
                contentList.append(qMakePair(currentText, QImage()));
            } else if (reader.name().toString() == "img") {
                contentList.append(qMakePair(QString(), currentImage));
                currentImage = QImage();
            }
        } else if (reader.isCharacters() && !reader.isWhitespace()) {
            currentText += reader.text().toString();
        }
    }

    return contentList;
}



