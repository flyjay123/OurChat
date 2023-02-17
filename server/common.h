#ifndef _COMMON_H_
#define _COMMON_H_
#include "DeThread.h"
//#include "chatTask.h"
#include "infos.h"
//#include "session.h"
//#include "CommandHandler.h"

//线程相关
#include<pthread.h>
//网络相关
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include <assert.h>
//c++
#include <iostream>
#include <map>
//数据库
#include <sqlite3.h>
//json
#include <nlohmann/json.hpp>

#define RET_OK 0
#define RET_ERROR -1
#define RET_AGAIN -2    //重新读取
#define RET_EXIT -3     //客户端退出


#ifndef FILENAME
#define FILENAME (__FILE__)
#endif

#ifndef FILEFUNCTION
#define FILEFUNCTION  (__FUNCTION__)
#endif

#ifndef FILELINE
#define FILELINE   (__LINE__)
#endif

#define ChatLog std::cout<<"[ "<<FILENAME<<":"<<FILELINE<<"]["<<FILEFUNCTION<<"]"

#define LOGINFO(format, ...)                                                         \
    {                                                                              \
        printf("[ %s : %d] [%s]>>" format, FILENAME,FILELINE, FILEFUNCTION ,##__VA_ARGS__); \
    }

using namespace std;
using json = nlohmann::json;

extern sqlite3 *db;

//De 协议包头
struct DeMessageHead{
    char mark[2];   // "DE" 认证deroy的协议
    char version;
    char encoded;   //0 不加密，1 加密
    int length;
};

struct DeMessagePacket
{
    /* data */
    int mode;  //1 请求,2 应答,3 消息通知
    int error; //0 成功,非0,对应的错误码

    int sequence;   //序列号
    int command;    //命令号
};


//命令枚举
enum commands{
    CommandEnum_Registe,
    CommandEnum_Login,
    CommandEnum_Logout,
    CommandEnum_GroupChat,
    CommandEnum_AddFriend,
    CommandEnum_delFriend,
    CommandEnum_PrivateChat,
    CommandEnum_CreateGroup,
    CommandEnum_GetGroupList,
    CommandEnum_GetGroupInfo,
    CommandEnum_GetFriendInfo,
};

#endif