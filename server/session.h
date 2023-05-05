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
    void sendMsg(vector<int> fds, json &j);
    json recvMsg();
    int handleMsg(json msg);

    //获取群成员账号
    vector<int> getGroupMember(int groupId);
    //获取群列表
    vector<int> getGroupList(int account);
    //获取好友列表
    vector<int> getFriendList(int account);
    //根据账号获取在线好友fd
    vector<int> getFriendFd(vector<int> friendList);
    int getFriendFd(int account);
    //发送系统消息
    void sendSystemMsg(int fd,string msg);
    //获取群主fd
    int getGroupOwnerFd(int groupId);

public:
    int GetSocket(){return m_socket;}
    string GetUserName(){return m_userName;}
    void SetUserName(string name){m_userName = name;}
    int GetAccount(){return m_account;}
    void SetAccount(int account){m_account = account;}
    UserInfo GetUserInfo(){return m_userInfo;}
    void SetUserInfo(UserInfo userInfo){m_userInfo = userInfo;}

    int GetIsLogin(){return m_isLogin;}
    void SetIsLogin(int isLogin){m_isLogin = isLogin;}

    vector<int> GetGroupList(){return m_groupList;}
    void SetGroupList(vector<int> groupList){m_groupList = groupList;}
    vector<int> GetGroupList(int account){return getGroupList(account);}

    vector<int> GetFriendList(){return m_friendList;}
    void SetFriendList(vector<int> friendList){m_friendList = friendList;}
    vector<int> GetFriendList(int account){return getFriendList(account);}
/*
public:
//操作数据库
    // Bind an integer value to the specified parameter index
    void bind(int index, int value) {
        m_statement.bind(index, value);
    }

    // Bind a string value to the specified parameter index
    void bind(int index, const std::string& value) {
        m_statement.bind(index, value);
    }

    // Execute the statement and return the number of rows affected
    int exec() {
        return m_statement.exec();
    }

    // Execute the statement and call the provided callback function for each row in the result set
    int exec(std::function<void(SQLite::Statement&)> callback) {
        int rowCount = 0;
        while (m_statement.executeStep()) {
            callback(m_statement);
            ++rowCount;
        }
        m_statement.reset();
        return rowCount;
    }
*/

private:
    void cleanSession();
    void noticeUserLogin(UserInfo*userInfo);
private:
    int m_socket;
    int m_account;          //会话账号
    string m_userName;    //用户名
    int m_isLogin;
    vector<int> m_groupList;    //群列表
    vector<int> m_friendList;   //好友列表

    UserInfo m_userInfo;

    Statement *m_statement;
};
#endif  