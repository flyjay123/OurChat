#ifndef INFOS_H
#define INFOS_H

#include <iostream>
#include <string>
using std::string;

typedef struct _user_info
{
    
  int account = 0;
  string password = "";
  string name = "";
  string sig = "";
  int online = 0;
  string icon = "";
    
} UserInfo;

typedef struct _login_info
{
    string cmd;
    int account;
    string password;
    string name;
} LoginInfo;

typedef struct _send_info
{
  string cmd;
  string info;
}SendInfo;

#endif