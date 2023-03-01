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
