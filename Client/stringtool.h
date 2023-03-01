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
};

#endif // STRINGTOOL_H
