#ifndef DATABASE_HANDLER_H
#define DATABASE_HANDLER_H

#include "common.h"
#include <mysql-cppconn-8/mysql/jdbc.h>

class DatabaseHandler {
private:
    //Members
    sql::Connection * connection;
    sql::Driver * driver;
    std::string userName;
    std::string password;
    std::string databaseName;
    std::string url;
    std::string errMsg;

    bool createDatabase();
    bool createTable();
    bool searchAccount(std::string accountNumber);
public:
    DatabaseHandler(std::string url, std::string userName, std::string password);
    ~DatabaseHandler();

    //Main Database Methods
    bool insert(account_data user);
    bool remove(std::string accountNumber);
    bool update(const std::string accountNumber, const std::string field, const std::string value);
    bool accountLookup(const std::string accountNumber, account_data * data);
};

#endif //DATABASE_HANDLER_H