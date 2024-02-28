#ifndef DATABASE_HANDLER_H
#define DATABASE_HANDLER_H

#include <iostream> 
#include <string>
#include <exception>
#include <sstream>
#include <sqlite3.h>
#include <stdio.h>
#include "Common.h"
#include <typeinfo>
#include <type_traits>


/*
* This class will handle all Database operations from
* creation of the database, handling insertion, deletion,
* and modifications, and opening and closing of database.
*/
class DatabaseHandler {
private:
	std::string databaseName;
	sqlite3* db;

	bool doesTableExist();
	std::string createInsertQuery(CUSTOMER customer);
	bool openConnection();
	bool closeConnection();

public:
	DatabaseHandler(std::string databaseName);
	~DatabaseHandler();

	//Function prototypes
	bool createDatabase();
	bool createTable();
	bool insertAccount(CUSTOMER customer);
	bool deleteAccount(std::string accountNumber);
	bool updateAccount(std::string accountNumber, int selection);
	bool lookupByName(std::string name);
	bool lookupByAccount(std::string accountNumber);
	bool updateDate(std::string newDate, std::string accountNumber);
	bool updateBalance(std::string accountNum, double newBalance);
	std::string getAccountNumber(std::string name);

	template <typename T>
	void getData(std::string accountNumber, int columnLoc, T *data);
};

template <typename T>
void DatabaseHandler::getData(std::string accountNumber, int columnLoc, T* data) {
	//Connect to database
	openConnection();

	//Get Data
	sqlite3_stmt* stmt;
	std::string query = "SELECT * FROM ACCOUNTS WHERE ACCNUM = '" + accountNumber + "';";

	int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &stmt, nullptr);
	if (result == SQLITE_OK) {
		result = sqlite3_step(stmt);
		if (result == SQLITE_ROW) {
			if (std::is_same<T, int>::value) {
				*data = sqlite3_column_int(stmt, columnLoc);
			}
			else if (std::is_same<T, std::string>::value) {
				*data = (T)sqlite3_column_text(stmt, columnLoc);
			}
			else {
				*data = (T)sqlite3_column_double(stmt, columnLoc);
			}
		}

		sqlite3_finalize(stmt);
	}

	closeConnection();
}

#endif
