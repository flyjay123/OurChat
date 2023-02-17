#include "../common.h"
#include "../session.h"
#include "../infos.h"

class CallbacksHandler
{
public:
    static int Search(void *data,int args_num,char **argv,char **argc);
    static int verifyAccountInDB(void *data, int args_num, char **argv, char **argc);
    static int IsExisted(void *data, int args_num, char **argv, char **argc);
    static int LoginReply(void *data, int args_num, char **argv, char **argc);
    
};
