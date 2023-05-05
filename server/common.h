#ifndef _COMMON_H_
#define _COMMON_H_
#include "DeThread.h"
// #include "chatTask.h"
#include "infos.h"
// #include "session.h"
// #include "CommandHandler.h"

// 线程相关
#include <pthread.h>
#include <mutex>
#include <thread>
// 网络相关
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <assert.h>
// c++
#include <iostream>
#include <map>
// 数据库
#include <SQLiteCpp/SQLiteCpp.h>
// json
#include <nlohmann/json.hpp>

#define RET_OK 0
#define RET_ERROR -1
#define RET_AGAIN -2 // 重新读取
#define RET_EXIT -3  // 客户端退出

#ifndef FILENAME
#define FILENAME (__FILE__)
#endif

#ifndef FILEFUNCTION
#define FILEFUNCTION (__FUNCTION__)
#endif

#ifndef FILELINE
#define FILELINE (__LINE__)
#endif

#define ChatLog std::cout << "[ " << FILENAME << ":" << FILELINE << "][" << FILEFUNCTION << "]"

#define LOGINFO(format, ...)                                                                 \
    {                                                                                        \
        printf("[ %s : %d] [%s]>>" format, FILENAME, FILELINE, FILEFUNCTION, ##__VA_ARGS__); \
    }

using namespace std;
using json = nlohmann::json;
using Database = SQLite::Database;
using Statement = SQLite::Statement;

extern SQLite::Database  db;
extern map<int, int> userMap;


// 命令枚举
enum commands
{
    cmd_regist = 0,
    cmd_login,
    cmd_logout,
    cmd_friend_search,
    cmd_add_friend_request,
    cmd_add_friend_response,
    cmd_friend_list,
    cmd_friend_chat,

    cmd_group_create,
    cmd_group_search,
    cmd_group_join_request,
    cmd_group_join_response,
    cmd_group_list,
    cmd_group_chat,
    cmd_group_member_list,
    cmd_group_member_add,
    cmd_group_member_del,
    cmd_set_icon
};

typedef struct _search_result
{
    int count = 0;
    std::vector<std::string> value;
    std::vector<UserInfo> infoRes;

public:
    void clear()
    {
        if (count > 0)
        {
            count = 0;
            infoRes.clear();
            value.clear();
        }
    }
} SearchResult;

#endif