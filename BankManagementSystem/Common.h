#ifndef COMMON_DATA_H
#define COMMON_DATA_H

#include <string>

struct CUSTOMER{
	std::string accountNumber;
	std::string name;
	std::string phone;
	std::string address;
	std::string dob;
	std::string date;
	int acctype;
	int age;
	double amount;
};

enum ACTION { EXIT, CREATE, UPDATE, REMOVE, DISPLAY, TRANSACTION };
enum ACCOUNT_TYPE { FIXED1 = 1, FIXED2, FIXED3, SAVING, CURRENT };
enum UpdateType { PHONE = 1, ADDRESS, AMOUNT };
enum LOOKUP { NAME = 1, NUMBER };
enum TRANSACTIONS { WITHDRAWL = 1, DEPOSIT };
enum DATA {ACCNUM = 1, NAMESTRING, AGE, PHONENUM, ADDRESSSTRING, DOB, DATE, ACCTYPE, BALANCE};

#endif