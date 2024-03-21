#include "databaseHandler.h"

#define DUPLICATE_ERROR 1062

DatabaseHandler::DatabaseHandler(std::string url, std::string userName, std::string password){
    url = "tcp://127.0.0.1:3306";
    databaseName = "accounts";
    
    //Establish a MySQL Connector/C++ driver instance
    this->driver = sql::mysql::get_driver_instance();

    //Establish a connection to the MySQL server
    this->connection = this->driver->connect(url, userName, password);

    //CreateDatabase
    if(!createDatabase()){
        std::cout << "[ERR] Failed to create database" << std::endl;
    }

    //CreateTable
    if(!createTable()){
        std::cout << "[ERR] Failed to create table" << std::endl;
    }
}

DatabaseHandler::~DatabaseHandler(){
    //Close Connection
    delete this->connection;
    std::cout << "Connection closed" << std::endl;
}

bool DatabaseHandler::createDatabase(){
    try{
        //Create a test database
        sql::Statement *stmt = connection->createStatement();
        stmt->execute("CREATE DATABASE IF NOT EXISTS " + databaseName);
        delete stmt;

        //Use the test database
        connection->setSchema(databaseName);

        return true;
    }catch(sql::SQLException &e){
        std::cout << "[ERR] " << e.what() << std::endl;
        return false;
    }
}

bool DatabaseHandler::createTable(){
    std::stringstream query;
    try{
        //Create a table
        sql::Statement *stmt = connection->createStatement();
        stmt = connection->createStatement();

        //Create query
        query << "CREATE TABLE IF NOT EXISTS accounts ( ID INT NOT NULL AUTO_INCREMENT PRIMARY KEY,";
        query << "name VARCHAR(255),";
        query << "account VARCHAR(255)," << "age INT," << "address VARCHAR(255),";
        query << "account_type INT," << "phone VARCHAR(20),";
        query << "balance DOUBLE," << "dob VARCHAR(255),"  << "date VARCHAR(255),";
        query << "UNIQUE (name, account));";

        stmt->execute(query.str());
        delete stmt;
        return true;
    }catch(sql::SQLException &e){
        std::cout << "[ERR] " << e.what() << std::endl;
        return false;   
    }
}

bool DatabaseHandler::insert(account_data user){
    try{
        //Create insertion query with data
        std::stringstream query;
        query << "INSERT INTO accounts ";
        query << "(name, account, age, address, account_type, phone, balance, dob, date) ";
        query << "VALUES ('" << user.name << "', '" << user.accountNumber << "', ";
        query << user.age << ", '" << user.address << "', " << user.accountType;
        query << ", '" << user.phoneNumber << "', " << user.balance << ", '" << user.dob;
        query << "', '" << user.date << "')"; 

        //Execute Query
        sql::Statement * statement = connection->createStatement();
        statement->execute(query.str());
        delete statement;
    } catch (sql::SQLException &e){
        if(e.getErrorCode() == DUPLICATE_ERROR){
            std::cout << "Account Already exists" << std::endl;
            return false;
        }else{
            std::cout << "[ERR] " << e.what() << std::endl;
            return false;
        }   
    }
    return true;
}

bool DatabaseHandler::remove(std::string accountNumber){
    try{
        std::stringstream query;
        query << "DELETE FROM accounts WHERE account = '" << accountNumber << "'";

        //Execute Query statement
        sql::Statement * statement = connection->createStatement();
        statement->execute(query.str());
        delete statement;
    }catch(sql::SQLException &e){
        std::cout << "[ERR] " << e.what() << std::endl;
        return false;
    }

    return true;
}

bool DatabaseHandler::update(const std::string accountNumber, const std::string field, const std::string value){
    try{
        //Prepare query statement
        std::stringstream query;
        query << "UPDATE accounts SET ";
        if(field == "phone_number"){
            query << "phone = '" << value << "' ";
        }else if(field == "address"){
            query << "address = '" << value << "' ";
        }else if(field == "balance"){
            query << "balance = " << value << " ";
        }else if(field == "name"){
            query << "name = '" << value << "' ";
        }else{
            std::cout << "[err] Invalid Field: " << field << std::endl;
            return false;
        }
        query << "WHERE account = '" << accountNumber << "'";

        //Execute Statement
        sql::Statement * statement = connection->createStatement();
        statement->execute(query.str());
        delete statement;

    } catch (sql::SQLException &e){
        std::cout << "[ERR] " << e.what() << std::endl;
        return false;
    }
    return true;
}

bool DatabaseHandler::view(){
    return true;
}