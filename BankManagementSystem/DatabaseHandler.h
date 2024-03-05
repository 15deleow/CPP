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
#include <iomanip>
#include <cstdlib>   // for rand() and srand()
#include <ctime>     // for time()
#include <limits>

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
	void DisplayAccount(const std::string accountNumber = "");
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
			const unsigned char* text = sqlite3_column_text(stmt, columnLoc);

			//Handle the possible data types
			//constexpr ensures only the code relevant to the type of T is compiled and executed
			if constexpr (std::is_same_v<T, int>) {
				*data = text ? std::stoi(reinterpret_cast<const char*>(text)) : 0;
			}
			else if constexpr (std::is_same_v<T, double>) {
				*data = text ? std::stod(reinterpret_cast<const char*>(text)) : 0.0;
			}
			else if constexpr (std::is_same_v<T, std::string>) {
				*data = text ? std::string(reinterpret_cast<const char*>(text)) : std::string();
			}
		}

		sqlite3_finalize(stmt);
	}

	closeConnection();
}

#endif
