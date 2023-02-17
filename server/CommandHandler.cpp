#include "CommandHandler.h"
#include "dao/CallbacksHandler.h"
using json = nlohmann::json;
int SelectCount;

void CommandHandler::Regist(int account, string password, string name,Session* session)
{
    json msg;
    
    char sql[128];

    sprintf(sql,"insert into user values (%d,'%s','%s')",account,password.c_str(),name.c_str());
    int rs = sqlite3_exec(db,sql,0,0,0);
    if(rs == SQLITE_OK)
    {
        LOGINFO("account:%d,注册成功..\n",account);
        msg["cmd"]="yes";
    } 
    else
    {
        LOGINFO("account:%d,注册失败..\n",account);
        msg["cmd"]="no";
        msg["err"]="账号已存在";
    }
    session->sendMsg(msg);
}

void CommandHandler::Login(int account, string password, Session* session)
{
    json msg;
    char sql[128];
    SelectCount=0;
    sprintf(sql,"select * from user where account=%d and password='%s'",account,password.c_str()); 
    if(sqlite3_exec(db,sql,CallbacksHandler::LoginReply,0,0)!=SQLITE_OK)
    {
        std::cout << "sqlite exec error" << endl;
    }
    if(SelectCount == 0)
    {
        std::cout << "账号或密码错误" << std::endl;
        msg["cmd"] = "no";
        msg["err"] = "账号或密码错误";
    }
    else
    {
        std::cout << "登录成功" << std::endl;
        msg["cmd"] = "yes";
    }
    SelectCount=0;
    session->sendMsg(msg);
}




