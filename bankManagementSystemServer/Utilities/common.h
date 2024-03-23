#include <time.h>
#include <iostream>
#include <sstream>
#include <string>

struct account_data {
    std::string name;
    std::string accountNumber;
    int age;
    std::string address;
    int accountType;
    std::string phoneNumber;
    double balance;
    std::string dob;
    std::string date;
};

enum ACCOUNT_TYPE {FIXED1, FIXED2, FIXED3, SAVING, CURRENT};
enum TRANSACTION_TYPE {DEPOSIT = 1, WITHDRAW};