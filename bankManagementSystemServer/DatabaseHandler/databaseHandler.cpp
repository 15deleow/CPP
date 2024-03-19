#include "databaseHandler.h"

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
        query << "Name VARCHAR(255),";
        query << "account VARCHAR(255)," << "age INT," << "address VARCHAR(255),";
        query << "account_type INT," << "phone VARCHAR(20),";
        query << "balance DOUBLE," << "dob DATE,"  << "date DATE);";

        stmt->execute(query.str());
        delete stmt;
        return true;
    }catch(sql::SQLException &e){
        std::cout << "[ERR] " << e.what() << std::endl;
        return false;   
    }
}

bool DatabaseHandler::insert(){
    
}