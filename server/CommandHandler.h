#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H
#include "session.h"

class CommandHandler
{
    
public:
    static void Regist(int account, std::string password, std::string name,Session* session);
    static void Login(int account,std::string password, Session* session);
    static void Search(std::string info,Session* session);
    static void GroupSearch(std::string info,Session* session);
    static void AddFriendRequest(UserInfo info,int account,std::string sendmsg,Session* session);
    static void FriendList(int account, Session* session);
    static void GroupList(int account, Session* session);
    static void GroupJoinReguest(UserInfo info,int group_account,std::string group_name,std::string sendmsg,Session* session);

    //static void PChat(int account,std::string sendmsg,Session* session);
    static void GroupChat(int account,std::string sendmsg,int group_account ,Session* session);
    static void GroupMemberList(int account,Session* session);
};



#endif