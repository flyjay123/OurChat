#ifndef _SESSION_H_
#define _SESSION_H_

#include <stdio.h>
#include "common.h"


class Session{
public:
    Session(int socket);
    ~Session();

public:
    void sendMsg(json &j);
    void sendMsg(int fd, json &j);
    json recvMsg();
    int handleMsg(json msg);

public:
    int GetSocket(){return m_socket;}
    string GetUserName(){return m_userName;}
    void SetUserName(string name){m_userName = name;}
    SearchResult *GetResult(){return m_result;}

    SearchResult *m_result;

private:
    void cleanSession();
    void noticeUserLogin(UserInfo*userInfo);
private:
    int m_socket;
    int m_account;          //会话账号
    string m_userName;    //用户名
    int m_isLogin;
    char *m_head;			//接收到的头信息
    int m_bufLen;			//需要接收消息的长度
    int m_readPos;			//当前读取到的长度
    char *m_body;			//接收到的body
    bool m_isFinish;        //接收消息,用来判断是否接收完全
    
    // mapFriendInfo m_friendInfoMap;
};
#endif  