#ifndef BANK_MANAGEMENT_SYSTEM_H
#define BANK_MANAGEMENT_SYSTEM_H

#include "DatabaseHandler.h"
#include <regex>

class BankSystemManager {
private:
	DatabaseHandler dbHandler;

	//Main Methods
	void createAccount();
	void deleteAccount();
	void updateAccount();
	void transaction();
	void displayAccounts();
	void displayCustomerAccount();

	//Helper Methods
	void clearScreen();
	void getCustomerData(CUSTOMER *data);
	void printMainMenu();
	int menu();	
	int getAccountType();
	int getCurrentAge(int yearOfBirth);
	void withdraw(std::string accountNumber);
	void deposit(std::string accountNumber);
	void pauseConsole();
	void printBanner(std::string title);
	std::string generateAccountNumber();
	std::string getHomeAddress();
	std::string getCurrentDate();
public:
	BankSystemManager(const std::string& dbName);
	void run();
};

#endif
