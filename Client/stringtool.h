#ifndef STRINGTOOL_H
#define STRINGTOOL_H
#include<QString>

class StringTool
{
public:
    StringTool();

    static QString Str2QStr(std::string str);
};

#endif // STRINGTOOL_H
