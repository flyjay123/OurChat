#ifndef DB_HANDLER_H
#define DB_HANDLER_H

#include <vector>
#include "../common.h"

class DBHandler {

public:
    static bool verifyAccount(std::string user, std::string password);
};

#endif