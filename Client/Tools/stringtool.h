#ifndef STRINGTOOL_H
#define STRINGTOOL_H
#include<QString>
#include <QTime>
#include <QImage>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

class StringTool
{
public:
    StringTool();

    static QString Str2QStr(std::string str);
    static QString MergePushMsg(QDateTime  dateTime, QString name, QString text);
    //flag: 0 self 1 other
    static QString MergeSendTimeMsg(QDateTime  dateTime, int flag,QString name = "");
    static QString Int2QStr(int num);
    static QImage GetImageFromHtml(const QString& html);
};

#endif // STRINGTOOL_H
