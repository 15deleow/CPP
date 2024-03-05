#include "DatabaseHandler.h"

// Callback function for SQLite3 methods
static int sqliteErrorCallback(void* data, int errorCode, const char* errorMessage) {
    std::cerr << "SQLite error (" << errorCode << "): " << errorMessage << std::endl;
    return 0;
}
int lookupCallback(void* data, int argc, char** argv, char** azColName) {
    // This callback is used only to check if the query returned any rows
    // If it returns at least one row, it means the entry exists, so return zero
    return 0;
}

DatabaseHandler::DatabaseHandler(std::string databaseName) {
    //Convert name of database to a C string for sqlite methods
    DatabaseHandler::databaseName = databaseName;

    //Create Database
    createDatabase();
}

DatabaseHandler::~DatabaseHandler() {
    //Close the database
    sqlite3_close(this->db);
    std::cout << "Database successfully closed!" << std::endl;
}

bool DatabaseHandler::doesTableExist() {
    bool databaseExists = false;
    sqlite3_stmt* stmt;
    const char* query = "SELECT name FROM sqlite_master WHERE type='table' AND name='ACCOUNTS'";

    int result = sqlite3_prepare_v2(this->db, query, -1, &stmt, nullptr);
    if (result == SQLITE_OK) {
        result = sqlite3_step(stmt);
        if (result == SQLITE_ROW) {
            databaseExists = true;
        }
        sqlite3_finalize(stmt);
    }

    return databaseExists;
}

bool DatabaseHandler::createDatabase() {
    int exit = 0;

    //Attempt to open existing database or create a new one if it does not exist
    if (sqlite3_open(this->databaseName.c_str(), &this->db) != SQLITE_OK) {
        std::cout << "Failed to create new database and open existing database" << std::endl;
        return false;
    }

    sqlite3_exec(this->db, "PRAGMA foreign_keys = ON;", NULL, NULL, NULL); // Enable foreign key support
    sqlite3_exec(this->db, "BEGIN TRANSACTION;", NULL, NULL, NULL); // Begin transaction

    //Create table
    createTable();

    sqlite3_exec(this->db, "COMMIT;", NULL, NULL, NULL); // Commit transaction
    sqlite3_exec(this->db, "PRAGMA synchronous = NORMAL;", NULL, NULL, NULL); // Set synchronous mode

    sqlite3_close(this->db);
    return true;
}

bool DatabaseHandler::createTable() {
    //Create table if it does not exist
    std::string query = "CREATE TABLE IF NOT EXISTS ACCOUNTS("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
        "ACCNUM TEXT NOT NULL UNIQUE,"
        "NAME TEXT NOT NULL,"
        "AGE INTEGER,"
        "PHONE TEXT,"
        "ADDRESS TEXT,"
        "DOB TEXT,"
        "DATE TEXT,"
        "ACCTYPE INTEGER,"
        "AMOUNT REAL);";

    try {
        char* errorMessage;
        if (sqlite3_exec(this->db, query.c_str(), NULL, 0, &errorMessage) != SQLITE_OK) {
            std::cerr << "Error Creating Table: " << errorMessage << std::endl;
            sqlite3_free(errorMessage);
        }
    }
    catch (std::exception &e ) {
        std::cerr << e.what() << std::endl;
    }

    return true;
}

bool DatabaseHandler::insertAccount(CUSTOMER customer) {
    //Convert customer data into string
    std::string stmt = createInsertQuery(customer);

    //Open database
    openConnection();

    //Execute Insertion
    if (sqlite3_exec(this->db, stmt.c_str(), NULL, 0, NULL) != SQLITE_OK) {
        std::cout << "ERROR INSERT: " << sqlite3_errmsg(this->db) << std::endl;
        return false;
    }
    else {
        std::cout << "Account has been entered into database" << std::endl;
    }

    //Close connection to database
    closeConnection();

    return true;
}

bool DatabaseHandler::deleteAccount(std::string accountNumber) {
    //Open connection to database
    if (!openConnection()) {
        return false;
    }

    //Create query statement
    std::stringstream ss;
    ss << "DELETE FROM ACCOUNTS where ACCNUM = '" << accountNumber << "'" << ";";
    
    //Execute Deletion
    if (sqlite3_exec(this->db, ss.str().c_str(), NULL, NULL, NULL) != SQLITE_OK) {
        std::cerr << "[ERR] " << sqlite3_errmsg(this->db) << std::endl;
        return false;
    }

    //Close connection to database
    if (!closeConnection()) {
        return false;
    }

    return true;
}

bool DatabaseHandler::updateAccount(std::string accountNumber, int selection) {
    //Prepare Update QUERY
    std::stringstream ss;
    ss << "UPDATE ACCOUNTS set ";

    //Open Connection
    if (!openConnection()) {
        return false;
    }

    //Prompt user to decide what to update
    switch (selection) {
    case PHONE: {
        std::string input;
        std::cout << "Please enter new phone number: ";
        std::getline(std::cin, input);
        ss << "PHONE = '" << input << "' WHERE ACCNUM = '" << accountNumber << "';";
        break;
    }
    case ADDRESS: {
        std::string input;
        std::cout << "Please enter new home address: ";
        getline(std::cin, input);
        ss << "ADDRESS = '" << input << "' WHERE ACCNUM = '" << accountNumber << "';";
        break;
    }
    default:
        std::cout << "[ERR] Invalid Input" << std::endl;
        return false;
    }

    //Execute Update query
    if (sqlite3_exec(this->db, ss.str().c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cout << "[ERR] " << sqlite3_errmsg(this->db) << std::endl;
        return false;
    }

    //Close Connection
    if (!closeConnection()) {
        return false;
    }

    return true;
}

void DatabaseHandler::DisplayAccount(const std::string accountNumber) {
    sqlite3_stmt* stmt;
    std::string query;

    //Create query statement to get all accounts or a specific account
    if (accountNumber.empty()) {
        query = "SELECT * FROM ACCOUNTS ORDER BY name ASC;";
    }
    else {
        query = "SELECT * FROM ACCOUNTS WHERE ACCNUM = '" + accountNumber + "';";
    }

    //Connect to database
    openConnection();

    //Get data from database
    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &stmt, nullptr);
    if (result == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            //Get Specific data
            const unsigned char* accountNum = sqlite3_column_text(stmt, ACCNUM);
            const unsigned char* name = sqlite3_column_text(stmt, NAMESTRING);
            const unsigned char* phone = sqlite3_column_text(stmt, PHONENUM);
            int accountType = sqlite3_column_int(stmt, ACCTYPE);

            //Print out data
            std::cout << std::left << std::setw(20) << accountNum;
            std::cout << std::setw(20) << name;
            std::cout << std::setw(20) << phone;
            switch (accountType) {
            case FIXED1:
                std::cout << std::setw(15) << "Fixed 1" << std::endl;
                break;
            case FIXED2:
                std::cout << std::setw(15) << "Fixed 2" << std::endl;
                break;
            case FIXED3:
                std::cout << std::setw(15) << "Fixed 3" << std::endl;
                break;
            case SAVING:
                std::cout << std::setw(15) << "Saving" << std::endl;
                break;
            case CURRENT:
                std::cout << std::setw(15) << "Current" << std::endl;
                break;
            }
        }

        sqlite3_finalize(stmt);
    }
    else {
        std::cerr << "[ERR] " << sqlite3_errmsg(this->db) << std::endl;
    }

    closeConnection();
}

//Helper Methods
bool DatabaseHandler::openConnection() {
    if (sqlite3_open(this->databaseName.c_str(), &this->db) != SQLITE_OK) {
        std::cerr << "[ERR] " << sqlite3_errmsg(this->db) << std::endl;
        return false;
    }

    return true;
}

bool DatabaseHandler::closeConnection() {
    if (sqlite3_close(this->db) != SQLITE_OK) {
        std::cerr << "[ERR] " << sqlite3_errmsg(this->db) << std::endl;
        return false;
    }

    return true;
}

bool DatabaseHandler::lookupByName(std::string name) {
    bool accountExists = false;
    std::string query = "SELECT * FROM ACCOUNTS WHERE NAME = '" + name + "';";

    //Prepare the statement
    openConnection();
    
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &stmt, nullptr);

    if (result == SQLITE_OK) {
        result = sqlite3_step(stmt);
        if (result == SQLITE_ROW) {
            accountExists = true;
        }
        sqlite3_finalize(stmt);
    }

    closeConnection();

    return accountExists;
}

bool DatabaseHandler::lookupByAccount(std::string accountNumber) {
    bool accountExists = false;
    std::string query = "SELECT * FROM ACCOUNTS WHERE ACCNUM = '" + accountNumber + "';";

    //Prepare the statement
    openConnection();
    int result = sqlite3_exec(this->db, query.c_str(), lookupCallback,nullptr, nullptr);
    if (result != SQLITE_OK) {
        std::cerr << "[ERR] " << sqlite3_errmsg(this->db) << std::endl;
        return false;
    }

    closeConnection();

    return true;
}

bool DatabaseHandler::updateDate(std::string newDate, std::string accountNumber) {
    std::stringstream query;

    openConnection();

    query << "UPDATE ACCOUNTS SET DATE = '" << newDate << "' WHERE ACCNUM = '";
    query << accountNumber << "';";

    if (sqlite3_exec(this->db, query.str().c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cout << "[ERR] " << sqlite3_errmsg(this->db) << std::endl;
        return false;
    }

    closeConnection();

    return true;
}

bool DatabaseHandler::updateBalance(std::string accountNum, double newBalance) {
    std::stringstream query;
    query << "UPDATE ACCOUNTS SET AMOUNT = '" << newBalance << "' WHERE ACCNUM = '" << accountNum << "';";

    openConnection();

    if (sqlite3_exec(this->db, query.str().c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cout << "[ERR] " << sqlite3_errmsg(this->db) << std::endl;
        return false;
    }

    closeConnection();

    return true;
}

std::string DatabaseHandler::getAccountNumber(std::string name) {
    std::string accountNumber;
    bool accountExists = lookupByName(name);

    openConnection();

    if (accountExists) {
        sqlite3_stmt* stmt;
        std::string query = "SELECT * FROM ACCOUNTS WHERE NAME = '" + name + "';";

        int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &stmt, nullptr);
        if (result == SQLITE_OK) {
            result = sqlite3_step(stmt);
            if (result == SQLITE_ROW) {
                //Return account number
                accountNumber = (char *)sqlite3_column_text(stmt, 1);
            }
            else {
                accountNumber = "";
            }
            sqlite3_finalize(stmt);
        }
    }

    closeConnection();

    return accountNumber;
}

std::string DatabaseHandler::createInsertQuery(CUSTOMER customer) {
    std::stringstream ss;
    ss << "INSERT INTO ACCOUNTS (ACCNUM, NAME, AGE, PHONE, ADDRESS, DOB, DATE, ACCTYPE, AMOUNT) VALUES ("
        << "'" << customer.accountNumber << "', "
        << "'" << customer.name << "', "
        << customer.age << ", "
        << "'" << customer.phone << "', "
        << "'" << customer.address << "', "
        << "'" << customer.dob << "', "
        << "'" << customer.date << "', "
        << customer.acctype << ", "
        << customer.amount << ");";

    return ss.str();
}