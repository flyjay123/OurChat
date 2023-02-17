#include "session.h"
#include "CommandHandler.h"
using namespace std;
using json = nlohmann::json_abi_v3_11_2::json;
Session::Session(int socket)
{
    m_isLogin = -1;
    m_account = -1;
    m_socket = socket;
}

Session::~Session(){
    if (m_socket != -1){
        close(m_socket);
        m_socket = -1;
    }
    if(m_isLogin == 1){
        cleanSession();
        m_isLogin = -1;
    }
};


int Session::handleMsg(json msg)
{
    std::cout << "handling" << std::endl;
    string cmd = msg.at("cmd");
    std::cout << "cmd: " << cmd << std::endl;
    LOGINFO("handleMsg command:%s\n",cmd.c_str());
    if(cmd == "login")
    {
        string account = msg.at("account");
        string password = msg.at("password");
        CommandHandler::Login(stoi(account),password,this);
    }
    else if(cmd == "regist")
    {
        string account = msg.at("account");
        string password = msg.at("password");
        string name = msg.at("name");
        CommandHandler::Regist(stoi(account),password,name,this);
    }
    
    return 0;
}

//message is json_string, that is json.dump()
void Session::sendMsg(json &j){
    std::string msg = j.dump();
    int len = msg.length();
    char buffer[4];
    sprintf(buffer,"%d",len);

    send(m_socket, buffer, 4, 0);
    send(m_socket, msg.c_str(), len, 0);
}

json Session::recvMsg()
{
    int len = 0;
    char buffer[4];
    memset(buffer,0,4);
    while(!recv(m_socket, buffer, 4, 0)){}
    std::cout << "buffur: " << buffer << endl;
    len = atoi(buffer);
    char* msg = new char[len + 1];
    memset(msg, 0, len + 1);

    recv(m_socket,msg,len,MSG_WAITALL);
    json j ;
    try{
        j = json::parse(msg);
    }
    catch(nlohmann::json_abi_v3_11_2::detail::parse_error& e)
    {
        std::cerr << "Failed to parse JSON: " << e.what() << std::endl;
        j["cmd"]="parseerror";
    }
    
    std::cout << "msg: " << msg << endl;
    delete[] msg;
    handleMsg(j);
    return j;
}

void Session::cleanSession(){
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