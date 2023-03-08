#include "session.h"
#include "CommandHandler.h"
#include <unistd.h>
using namespace std;
using json = nlohmann::json_abi_v3_11_2::json;
Session::Session(int socket)
{
    m_isLogin = -1;
    m_account = -1;
    m_socket = socket;
    m_result = new SearchResult;
}

Session::~Session()
{
    if (m_socket != -1)
    {
        close(m_socket);
        m_socket = -1;
    }
    if (m_isLogin == 1)
    {
        cleanSession();
        m_isLogin = -1;
    }
    delete m_result;
    m_result = nullptr;
};

int Session::handleMsg(json msg)
{
    usleep(100000);
    string cmd = msg.at("cmd");
    LOGINFO("handleMsg command:%s\n", cmd.c_str());

    if (cmd == "login")
    {
        string account = msg.at("account");
        string password = msg.at("password");
        
        try{
            int a = stoi(account,0,0);
            CommandHandler::Login(a, password, this);
        }
        catch(std::invalid_argument &e)
       {
            return 0;
       }
       
    }
    else if (cmd == "regist")
    {
        string account = msg.at("account");
        string password = msg.at("password");
        string name = msg.at("name");
        try{
            int a = stoi(account,0,0);
            CommandHandler::Regist(a, password, name, this);
        }
       catch(std::invalid_argument &e)
       {
            return 0;
       }
    }
    else if (cmd == "friend-search")
    {
        string info = msg.at("search-info");
        CommandHandler::Search(info, this);
    }
    else if (cmd == "addfriend-req")
    {
        string account = msg.at("account");
        string sendmsg = msg.at("sendmsg");
        CommandHandler::AddFriend(stoi(account), sendmsg, this);
    }
    else if (cmd == "friend-list")
    {
        usleep(400000);
        string account = msg.at("account");
        CommandHandler::FriendList(stoi(account), this);
    }
    else if (cmd == "pchat")
    {
        cout << "handle pchat" << endl;
        string account = msg.at("account");
        string sendmsg = msg.at("sendmsg");

        for (auto it = userMap.begin(); it != userMap.end(); it++)
        {
            if (it->second == stoi(account))
            {
                int fd = it->first;
                sendMsg(fd, msg);
            }
        }
    }

    return 0;
}

void Session::sendMsg(json &j)
{
    std::string msg = j.dump();
    int len = msg.length();
    char buffer[4];
    memcpy(buffer, &len, sizeof(len));

    char *message = new char[4 + len];
    sprintf(message, "%04d%s", len, msg.c_str());

    // 使用 send 函数发送消息
    send(m_socket, message, len + 4, 0);

    // send(m_socket, buffer, 4, 0);
    // send(m_socket, msg.c_str(), len, 0);
    std::cout << "send: " << message << std::endl;
    delete[] message;
    message = nullptr;
}

void Session::sendMsg(int fd, json &j)
{

    std::string msg = j.dump();
    int len = msg.length();
    char buffer[4];
    memcpy(buffer, &len, sizeof(len));

    char *message = new char[4 + len];
    sprintf(message, "%04d%s", len, msg.c_str());

    // 使用 send 函数发送消息
    send(fd, message, len + 4, 0);

    // send(m_socket, buffer, 4, 0);
    // send(m_socket, msg.c_str(), len, 0);
    std::cout << "send: " << message << std::endl;
    delete[] message;
    message = nullptr;
}

json Session::recvMsg()
{
    json j;
    int len = 0;
    int res = 0;
    char buffer[4];
    memset(buffer, 0, 4);
    while (1)
    {
        res = recv(m_socket, buffer, 4, 0);
        if (res > 0)
            break;
        if (res <= 0)
        {
            if (errno == EWOULDBLOCK || errno == EAGAIN)
                continue;
            j["cmd"] = "logout";
            return j;
        }
        if (buffer[0] == '\0')
            continue;
    }
    len = atoi(buffer);
    char *msg = new char[len + 1];
    memset(msg, 0, len + 1);

    recv(m_socket, msg, len, MSG_WAITALL);
    std::cout << "rsvmsg = " << msg << std::endl;

    try
    {
        j = json::parse(msg);
    }
    catch (nlohmann::json_abi_v3_11_2::detail::parse_error &e)
    {
        std::cerr << "Failed to parse JSON: " << e.what() << std::endl;
        std::cout << "error json msg : " << msg << endl;
        j["cmd"] = "parseerror";
    }

    delete[] msg;
    msg = nullptr;
    handleMsg(j);
    return j;
}

void Session::cleanSession()
{
    /*
    mapUserInfo::iterator iter = g_UserInfoMap.find(m_account);
    if (iter == g_UserInfoMap.end())
    {
        LOGINFO("出错了..\n");
    }
    else
    {
        UserInfo* delUserInfo = iter->second;
        g_UserInfoMap.erase(iter);
        if(delUserInfo!=NULL){
            free(delUserInfo);
            delUserInfo =NULL;
        }
        LOGINFO("会话关闭成功..\n在线用户数量:%d\n",g_UserInfoMap.size());
    }
    */
}