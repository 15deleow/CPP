#ifndef BANK_MANAGEMENT_SYSTEM_H
#define BANK_MANAGEMENT_SYSTEM_H

#include "DatabaseHandler.h"

class BankSystemManager {
private:
	DatabaseHandler dbHandler;

	void createAccount();
	void deleteAccount();
	void updateAccount();
	void transaction();

	void clearScreen();
	void getCustomerData(CUSTOMER *data);
	void printMainMenu();
	int menu();
	std::string generateAccountNumber();
	std::string getHomeAddress();
	std::string getCurrentDate();
	int getAccountType();
	int getCurrentAge(int yearOfBirth);
	void withdraw(std::string accountNumber);
	void deposit(std::string accountNumber);
public:
	BankSystemManager(const std::string& dbName);
	void run();
};

#endif
