#include "../common.h"
#include "../session.h"
#include "../infos.h"

class CallbacksHandler
{
public:
    static int SearchReply(void *data,int args_num,char **argv,char **argc);
    static int LoginReply(void *data, int args_num, char **argv, char **argc);
    static int FriendListReply(void *data, int args_num, char **argv, char **argc);
};

