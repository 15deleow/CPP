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
public:
    DatabaseHandler(std::string url, std::string userName, std::string password);
    ~DatabaseHandler();
};