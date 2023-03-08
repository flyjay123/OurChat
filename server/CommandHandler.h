#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H
#include "session.h"

class CommandHandler
{
    
public:
    static void Regist(int account, std::string password, std::string name,Session* session);
    static void Login(int account,std::string password, Session* session);
    static void Search(std::string info,Session* session);
    static void AddFriend(int account,std::string sendmsg,Session* session);
    static void FriendList(int account, Session* session);
};



#endif