#ifndef INFOS_H
#define INFOS_H

#include <iostream>
#include <string>
using std::string;

struct UserInfo{
int account;
std::string password;
std::string name;
};

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