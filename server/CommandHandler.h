#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H
#include "session.h"

class CommandHandler
{
    
public:
    static void Regist(int account, string password, string name,Session* session);
    static void Login(int account,std::string password, Session* session);
};



#endif