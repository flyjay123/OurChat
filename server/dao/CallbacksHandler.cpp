#include "CallbacksHandler.h"
#include "../common.h"

using json = nlohmann::json;
extern int SelectCount;
int CallbacksHandler::Search(void *data,int args_num,char **argv,char **argc)
{ 
   for(int i=0;i<args_num;i++){

   }
   cout<<endl;
   return 0;
}

int CallbacksHandler::verifyAccountInDB(void *data, int args_num, char **argv, char **argc) {
    if(args_num > 1) {
        return false;
    }
    if(args_num == 0) {
        return false;
    }

    return strcmp((const char*)data, argv[0]);
}

int CallbacksHandler::IsExisted(void *data, int args_num, char **argv, char **argc)
{
    if(args_num == 0)
    {
        *(int*)data = 0;
        return false;
    }
    *(int*)data = 1;
    return true;
}

int CallbacksHandler::LoginReply(void *data, int args_num, char **argv, char **argc)
{
    std::cout << "args_num = " <<args_num << std::endl;
    if(args_num == 0)
    {
        return 0;
    }

    SelectCount=1;
    return 1;
}
