#include "DBHandler.h"
#include "CallbacksHandler.h"



bool DBHandler::verifyAccount(std::string user, std::string password) {
    /*
    sqlite3 *db;

    char* errMsg;
    int rc = sqlite3_open("./user.db", &db);

    if(rc != SQLITE_OK)
    {
        cout << "Open .db file failed" << endl;
        return -1;
    }

    const char* pwd = password.c_str();

    string sql = "select account password from user where account = " + user ;
    int rs = sqlite3_exec(db, sql.c_str(), verifyAccountInDB, (void*)pwd, &errMsg);
    if(rs != SQLITE_OK)
    {
        cout << "Insert failed" << endl;
    }
    sqlite3_close(db);
    */
    return true;
}