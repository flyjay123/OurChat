#define SQLITECPP_ENABLE_ASSERT_HANDLER
#include "CommandHandler.h"
using json = nlohmann::json;

// extern SearchResult accountResult;
using namespace std;

void CommandHandler::Regist(int account, string password, string name, Session *session)
{
    json msg;
    msg["cmd"] = cmd_regist;
    Statement query(db, "insert into user values (?,?,?,'hello',0,':/Icons/src/QQIcon/icon.jpg')");
    query.bind(1, account);
    query.bind(2, password);
    query.bind(3, name);
    int res = query.exec();
    if (res == 1)
    {
        LOGINFO("account:%d,注册成功..\n", account);
        msg["res"] = "yes";
        Statement query1(db, "insert into friend values (10000,?)");
        query1.bind(1, account);
        query1.exec();
    }
    else
    {
        LOGINFO("account:%d,注册失败..\n", account);
        msg["res"] = "no";
        msg["err"] = "账号已存在";
    }
    session->sendMsg(msg);
}

void CommandHandler::Login(int account, string password, Session *session)
{
    json msg;
    msg["cmd"] = cmd_login;
    Statement query(db, "select name,signature,icon from user where account=? and password=?");
    query.bind(1, account);
    query.bind(2, password);
    // 执行query语句获取查询结果,判断查询结果，如果查询结果为空，说明账号或密码错误，否则登录成功
    if (!query.executeStep())
    {
        std::cout << "账号或密码错误" << std::endl;
        msg["res"] = "no";
        msg["err"] = "账号或密码错误";
    }
    else
    {
        std::cout << "登录成功" << std::endl;
        msg["res"] = "yes";
        for(int i = 0;i<3;i++)
        {
            msg["info"][i] = query.getColumn(i).getString();
        }
        UserInfo info = {account, password, msg["info"][0], msg["info"][1], 1, msg["info"][2]};
        session->SetUserInfo(info);
        userMap[session->GetSocket()] = account;
        session->SetAccount(account);
        session->SetIsLogin(true);
        Statement query(db, "update user set online=1 where account=?");
        query.bind(1, account);
        query.exec();
    }
    session->sendMsg(msg);
}

void CommandHandler::Search(std::string info, Session *session)
{
    json msg;
    int count = 0;
    int account = 0;
    try
    {
        account = std::stoi(info);
    }
    catch (const std::invalid_argument &e)
    {
        account = 0;
    }
    msg["cmd"] = cmd_friend_search;
    // 查找user表中int类型的account或string类型的name等于info的account
    Statement query(db, "select account,name from user where account=? or name=?");
    query.bind(1, account);
    query.bind(2, info);
    while (query.executeStep())
    {
        msg["msglist"][count]["account"] = query.getColumn(0).getString();
        msg["msglist"][count]["name"] = query.getColumn(1).getString();
        count++;
    }

    msg["count"] = count;
    session->sendMsg(msg);
}

void CommandHandler::GroupSearch(std::string info, Session *session)
{
    json msg;
    int count = 0;
    int account = 0;
    try
    {
        account = std::stoi(info);
    }
    catch (const std::invalid_argument &e)
    {
        account = 0;
    }
    msg["cmd"] = cmd_group_search;
    // 查找user表中int类型的account或string类型的name等于info的account
    Statement query(db, "select group_account,group_name from 'group' where group_account=? or group_name=?");
    query.bind(1, account);
    query.bind(2, info);
    while (query.executeStep())
    {
        msg["msglist"][count]["account"] = query.getColumn(0).getString();
        msg["msglist"][count]["name"] = query.getColumn(1).getString();
        count++;
    }

    msg["count"] = count;
    session->sendMsg(msg);
}

void CommandHandler::AddFriendRequest(UserInfo info, int account, std::string sendmsg, Session *session)
{
    // 判断是否已经是好友
    Statement query(db, "select * from friend where (user1=? and user2=?) or (user1=? and user2=?)");
    query.bind(1, info.account);
    query.bind(2, account);
    query.bind(3, account);
    query.bind(4, info.account);
    if (query.executeStep())
    {
        json msg;
        msg["cmd"] = cmd_add_friend_request;
        msg["res"] = "no";
        msg["err"] = "已经是好友";
        session->sendMsg(msg);
        return;
    }
    else
    {
        json msg;
        msg["cmd"] = cmd_add_friend_request;
        msg["sender"] = info.account;
        // 根据账号查找fd
        int fd = session->getFriendFd(account);
        if (fd <= 0)
        {
            msg["res"] = "no";
            msg["err"] = "对方不在线";
            session->sendMsg(msg);
            return;
        }
        else
        {
            msg["res"] = "yes";
            msg["sendmsg"] = sendmsg;
            msg["name"] = info.name;
            msg["sig"] = info.sig;
            msg["icon"] = info.icon;
            session->sendMsg(fd, msg);
        }
    }

    return;
}

void CommandHandler::FriendList(int account, Session *session)
{
    json msg;
    int count = 0;
    msg["cmd"] = cmd_friend_list;
    cout << __LINE__ << endl;
    Statement query(db, "select account,name,signature,online,icon from user where account in \
    (select user2 from friend where user1=? union select user1 from friend where user2=?)");
    cout << __LINE__ << endl;
    query.bind(1, account);
    query.bind(2, account);
    while (query.executeStep())
    {
        msg["msglist"][count]["account"] = query.getColumn(0).getInt();
        msg["msglist"][count]["name"] = query.getColumn(1).getString();
        msg["msglist"][count]["sig"] = query.getColumn(2).getString();
        msg["msglist"][count]["online"] = query.getColumn(3).getInt();
        msg["msglist"][count]["icon"] = query.getColumn(4).getString();
        count++;
    }
    msg["count"] = count;
    if(account == session->GetAccount())
        session->sendMsg(msg);
    else
        session->sendMsg(session->getFriendFd(account), msg);
    return;
}

void CommandHandler::GroupList(int account, Session *session)
{

    json msg;
    int count = 0;
    msg["cmd"] = cmd_group_list;
    cout << __LINE__ << endl;
    Statement query(db, "select group_account,group_name from 'group' where group_account in (select group_account from member where member_id=?)");
    cout << __LINE__ << endl;
    query.bind(1, account);
    cout << __LINE__ << endl;
    while (query.executeStep())
    {
        msg["msglist"][count]["account"] = query.getColumn(0).getInt();
        msg["msglist"][count]["name"] = query.getColumn(1).getString();
        count++;
    }
    msg["count"] = count;
    if(account == session->GetAccount())
        session->sendMsg(msg);
    else
        session->sendMsg(session->getFriendFd(account), msg);
    return;
}

void CommandHandler::GroupChat(int account, std::string sendmsg, int group_account, Session *session)
{
    vector<int> friendList = session->getGroupMember(group_account);
    vector<int> fds = session->getFriendFd(friendList);
    json msg;
    msg["cmd"] = cmd_group_chat;
    msg["sender"] = account;
    msg["msg"] = sendmsg;
    msg["groupAccount"] = group_account;
    msg["time"] = time(NULL);

    // 找到fds中自己的fd，然后从fds中删除
    int selfFd = session->GetSocket();
    auto it = find(fds.begin(), fds.end(), selfFd);
    if (it != fds.end())
    {
        fds.erase(it);
        cout << "skip self" << endl;
    }
    session->sendMsg(fds, msg);
}

void CommandHandler::GroupMemberList(int account, Session *session)
{
    json msg;
    int count = 0;
    msg["cmd"] = cmd_group_member_list;
    msg["groupAccount"] = account;
    Statement query(db, "select member_id,group_nickname from member where group_account==?");
    query.bind(1, account);
    while (query.executeStep())
    {
        msg["msglist"][count]["account"] = query.getColumn(0).getString();
        msg["msglist"][count]["name"] = query.getColumn(1).getString();
        count++;
    }
    msg["count"] = count;
    session->sendMsg(msg);
}

void CommandHandler::GroupJoinReguest(UserInfo info, int group_account, std::string group_name, std::string sendmsg, Session *session)
{
    json msg;
    msg["cmd"] = cmd_group_join_request;
    msg["sender"] = info.account;
    msg["groupAccount"] = group_account;
    msg["groupName"] = group_name;
    // 根据账号查找fd
    int fd = session->getGroupOwnerFd(group_account);
    if (fd <= 0)
    {
        msg["res"] = "no";
        msg["err"] = "群主不在线";
        session->sendMsg(msg);
        return;
    }
    else
    {
        msg["res"] = "yes";
        msg["msg"] = sendmsg;
        msg["name"] = info.name;
        msg["sig"] = info.sig;
        msg["icon"] = info.icon;
        session->sendMsg(fd, msg);
    }
}
