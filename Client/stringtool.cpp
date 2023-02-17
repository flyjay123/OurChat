#include "stringtool.h"

StringTool::StringTool()
{

}

QString StringTool::Str2QStr(std::string str)
{
    QString qs = QString::fromStdString(str);
    return qs;
}
