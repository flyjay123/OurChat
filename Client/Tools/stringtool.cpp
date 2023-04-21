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

