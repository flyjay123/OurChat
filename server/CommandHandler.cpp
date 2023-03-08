#include "CommandHandler.h"
#include "dao/CallbacksHandler.h"
using json = nlohmann::json;

//extern SearchResult accountResult;
using namespace std;

void CommandHandler::Regist(int account, string password, string name,Session* session)
{
    json msg;
    char sql[128];
    msg["cmd"]="regist";
    sprintf(sql,"insert into user values (%d,'%s','%s')",account,password.c_str(),name.c_str());
    int rs = sqlite3_exec(db,sql,0,0,0);
    if(rs == SQLITE_OK)
    {
        LOGINFO("account:%d,注册成功..\n",account);
        msg["res"]="yes";
    } 
    else
    {
        LOGINFO("account:%d,注册失败..\n",account);
        msg["res"]="no";
        msg["err"]="账号已存在";
    }
    session->sendMsg(msg);
}

void CommandHandler::Login(int account, string password, Session* session)
{
    SearchResult* searchRes = session->GetResult();
    searchRes->clear();
    json msg;
    msg["cmd"] = "login";
    char sql[128];
    sprintf(sql,"select * from user where account=%d and password='%s'",account,password.c_str()); 
    if(!sqlite3_exec(db,sql,CallbacksHandler::LoginReply,searchRes,0))
    {
        std::cout << "账号或密码错误" << std::endl;
        msg["res"] = "no";
        msg["err"] = "账号或密码错误";
    }
    else
    {
        if(searchRes->count)
        {
            session->SetUserName(searchRes->value[0]);
            std::cout << "登录成功" << std::endl;
            msg["res"] = "yes";
            msg["name"] = session->GetUserName();
            userMap[session->GetSocket()] = account;
            sprintf(sql,"insert into friend values(10000,%d)",account);
            sqlite3_exec(db,sql,0,0,0);
        }
        
    }
    session->sendMsg(msg);
}

void CommandHandler::Search(std::string info,Session* session)
{
    SearchResult* searchRes = session->GetResult();
    searchRes->clear();
    json msg;
    char sql[128];
    std::cout << __LINE__ << std::endl;
    try{
        sprintf(sql,"select account,name from user where account=%d or name ='%s'",stoi(info),info.c_str());
    }
    catch (std::invalid_argument &e)
    {
        sprintf(sql,"select account,name from user where name='%s'",info.c_str());
    }
    int res = sqlite3_exec(db,sql,CallbacksHandler::SearchReply,searchRes,0);
    if(searchRes->count)
    {
        std::cout << "size > 0" << std::endl;
        msg["cmd"]="yes";
        int count = 0;
       for(int i=0;i<searchRes->value.size();i+=2)
       {
            msg["msglist"][i/2]["account"]=searchRes->value[i];
            msg["msglist"][i/2]["name"]=searchRes->value[i+1];
            std::cout << searchRes->value[i] << " " << searchRes->value[i+1] << std::endl;
       }
    }
    session->sendMsg(msg);
}

void CommandHandler::AddFriend(int account,std::string sendmsg,Session* session)
{
    return ;
}

void CommandHandler::FriendList(int account, Session* session)
{
    SearchResult* searchRes = session->GetResult();
    searchRes->clear();
    //result.value.shrink_to_fit();
    json msg;
    msg["cmd"]="friend-list";
    char sql[512];
    sprintf(sql,
    "select * \
    from user\
    where user.account in (\
    select user2 from friend where user1=%d\
    union\
    select user1 from friend where user2=%d\
    );"
    ,account,account); 
    if(!sqlite3_exec(db,sql,CallbacksHandler::FriendListReply,searchRes,0))
    {

    }
    for(int i = 0; i < searchRes->count; i++)
    {
        msg["msglist"][i]["account"] = std::to_string(searchRes->infoRes[i].account);
        msg["msglist"][i]["name"] = searchRes->infoRes[i].name;
        msg["msglist"][i]["sig"] = searchRes->infoRes[i].sig;
    }
    msg["count"] = searchRes->count;
    session->sendMsg(msg); 
}




