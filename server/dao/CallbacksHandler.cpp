#include "CallbacksHandler.h"
#include "../common.h"

int CallbacksHandler::SearchReply(void *data,int args_num,char **argv,char **argc)
{ 
    if(args_num)
    {
        SearchResult *searchRes = (SearchResult*)data;
        searchRes->count++;
        string account = argv[0];
        string name = argv[1];
        searchRes->value.push_back(account); 
        searchRes->value.push_back(name); 
    }

   return 0;
}

int CallbacksHandler::LoginReply(void *data, int args_num, char **argv, char **argc)
{
    if(args_num)
    {
        SearchResult *searchRes = (SearchResult*)data;
        string name = argv[2];
        searchRes->value.push_back(name);
        searchRes->count++;
        return 1;
    }
    return 0;
}

int CallbacksHandler::FriendListReply(void *data, int args_num, char **argv, char **argc)
{
    if(args_num)
    {
        SearchResult *searchRes = (SearchResult*)data;
        UserInfo info;
        info.account = atoi(argv[0]);
        info.name = argv[2];
        info.sig = argv[3];
        
        searchRes->count++;
        searchRes->infoRes.emplace_back(info);
    }
    return 0;
}
