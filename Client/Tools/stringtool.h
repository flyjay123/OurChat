#ifndef STRINGTOOL_H
#define STRINGTOOL_H
#include<QString>
#include <QTime>

class StringTool
{
public:
    StringTool();

    static QString Str2QStr(std::string str);
    static QString MergePushMsg(QDateTime  dateTime, QString name, QString text);
    static QString Int2QStr(int num);
};

#endif // STRINGTOOL_H
