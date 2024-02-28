#include "BankManagementSystem.h"
#include <iomanip>
#include <cstdlib>   // for rand() and srand()
#include <ctime>     // for time()

//Helper Methods
void BankSystemManager::clearScreen() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

void BankSystemManager::printMainMenu() {
    // Print the title
    std::cout << std::setw(40) << std::setfill('=') << '\n';
    std::cout << std::setw(30) << std::setfill(' ') << "Banking System Options" << '\n';
    std::cout << std::setw(40) << std::setfill('=') << '\n';

    // Print the options
    std::cout << "1. Create Account\n";
    std::cout << "2. Update Account\n";
    std::cout << "3. Remove Account\n";
    std::cout << "4. Display Account\n";
    std::cout << "5. Transaction\n";
    std::cout << "0. Exit\n";
}

std::string BankSystemManager::generateAccountNumber() {
    std::string result;
    int length{ 12 };

    //Seed the random number generator
    srand(time(nullptr));
    for (int i = 0; i < length; ++i) {
        //Generate a random digit (0-9) and add it to the result string
        result += std::to_string(rand() % 10);
    }
    return result;
}

std::string BankSystemManager::getHomeAddress() {
    std::string streetName, city, state, zipcode;
    std::string homeAddress;

    std::cout << "Enter home address" << std::endl;;
    std::cout << "\tEnter Street Name: ";
    std::getline(std::cin, streetName);

    std::cout << "\tEnter city: ";
    std::getline(std::cin, city);

    std::cout << "\tEnter state: ";
    std::getline(std::cin, state);

    std::cout << "\tEnter zip code: ";
    std::getline(std::cin, zipcode);

    //Combine Strings to make Home Address
    homeAddress = streetName + ", " + city + ", " + state + ", " + zipcode;
    return homeAddress;
}

std::string BankSystemManager::getCurrentDate() {
    // Get the current time
    std::time_t currentTime = std::time(nullptr);

    // Convert the current time to struct tm
    std::tm* localTime = std::localtime(&currentTime);

    // Create a string stream to format the date
    std::ostringstream oss;

    // Format the date as mm/dd/yyyy
    oss << std::setw(2) << std::setfill('0') << 1 + localTime->tm_mon << "/";
    oss << std::setw(2) << std::setfill('0') << localTime->tm_mday << "/";
    oss << 1900 + localTime->tm_year;

    // Return the formatted date as a string
    return oss.str();
}

int BankSystemManager::getAccountType() {
    int accountType;

    std::cout << "ACCOUNT TYPES" << std::endl;
    std::cout << "1.FIXED1\n2.FIXED2\n3.FIXED3\n4.SAVING\n5.CURRENT\n";
    std::cout << "Select type of account: ";
    std::cin >> accountType;

    return accountType;
}

int BankSystemManager::getCurrentAge(int yearOfBirth) {
    // Get the current time
    std::time_t currentTime = std::time(nullptr);

    // Convert the current time to struct tm
    std::tm* localTime = std::localtime(&currentTime);

    // Extract the year from the struct tm
    int year = 1900 + localTime->tm_year;

    return year - yearOfBirth;
}

void BankSystemManager::getCustomerData(CUSTOMER *data) {
    //Generate unique account number
    data->accountNumber = generateAccountNumber();
    
    //Get Customer's Name
    std::cout << "Enter Full Name: " << std::flush;
    std::cin.ignore();
    std::getline(std::cin, data->name);
    
    //Get Customer's Phone Number
    std::cout << "Enter ten digit phone number: ";
    std::cin.ignore();
    std::getline(std::cin, data->phone);

    //Get Customer's Home Address
    data->address = getHomeAddress();

    //Get Customer's Date of Birth
    std::cout << "Enter date of birth <mm/dd/yyyy>: ";
    std::cin.ignore();
    std::getline(std::cin, data->dob);

    //Calculate User's Age
    int age = std::stoi(data->dob.substr(data->dob.length() - 4));
    data->age = getCurrentAge(age);

    //Get Current Date
    data->date = getCurrentDate();

    //Get Account Type
    data->acctype = getAccountType();

    //Get Initial Deposit
    std::cout << "Enter amount to deposit <0.00>: ";
    std::cin >> data->amount;
}

void BankSystemManager::withdraw(std::string accountNumber) {

}

void BankSystemManager::deposit(std::string accountNumber) {

}

//Main Methods
BankSystemManager::BankSystemManager(const std::string& dbName): dbHandler(dbName) {
	//Initializes the Database Handler object, which attempts to create the
	//accounts database if it does not exist
	return;
}

void BankSystemManager::createAccount() {
    //Get data from user
    CUSTOMER data;
    getCustomerData(&data);

    //Insert account into database
    this->dbHandler.insertAccount(data);
}

void BankSystemManager::deleteAccount() {
    int methodOption;
    bool accountExists = false;
    std::string accountNumber;
    
    //Prompt user for lookup method
    std::cout << "Delete Account" << std::endl;
    std::cout << "1. Search by name" << std::endl;
    std::cout << "2. Search by account number" << std::endl;
    std::cout << "Enter lookup method: ";
    std::cin >> methodOption;

    //Check if name exists based on lookup method
    if (methodOption == NAME) {
        //Search database by name
        std::string name;
        std::cout << "Enter Customer Name: ";
        std::cin.ignore();
        std::getline(std::cin, name);

        //Lookup account
        accountExists = this->dbHandler.lookupByName(name);
        
        //If it exists, retrieve account number and then delete
        if (accountExists == true) {
            accountNumber = this->dbHandler.getAccountNumber(name);
            if (this->dbHandler.deleteAccount(accountNumber) == false) {
                std::cout << "Failed to delete account!" << std::endl;
            }
            else {
                std::cout << "Account " << accountNumber << " deleted!" << std::endl;
            }
        }
        else {
            std::cout << "Account with the name, " << name << " does not exist!" << std::endl;
        }

    }else {
        //Search database by account number
        std::cout << "Enter Account Number: ";
        std::cin.ignore();
        std::getline(std::cin, accountNumber);

        accountExists = this->dbHandler.lookupByAccount(accountNumber);
        if (accountExists) {
            if (this->dbHandler.deleteAccount(accountNumber) == false) {
                std::cout << "Failed to delete account " << accountNumber << "!" << std::endl;
            }
            else {
                std::cout << "Account " << accountNumber << " successfully deleted!" << std::endl;
            }
        }
        else {
            std::cout << "Account " << accountNumber << " does not exist!" << std::endl;
        }
    }

    std::cin.get();
}

void BankSystemManager::transaction() {
    std::string response;
    std::string accountNumber;

    std::cout << "Does Customer know their account number: ";
    std::cin.ignore();
    std::getline(std::cin, response);
    
    //Get Account Number
    if (response == "yes") {
        std::cout << "Enter Account Number: ";
        std::getline(std::cin, accountNumber);
    }
    else {
        //Get account number using customer name
        std::string name;
        std::cout << "Enter Customer's Name: " << std::endl;
        std::getline(std::cin, name);

        accountNumber = this->dbHandler.getAccountNumber(name);
    }

    //Verify if Account is type savings or current
    int accType;
    this->dbHandler.getData(accountNumber, ACCTYPE, &accType);
    if (accType != SAVING && accType != CURRENT) {
        std::cout << "Cannot Withdrawl or Deposit in Fixed accounts" << std::endl;
        return;
    }

    //Prompt user for type of transaction
    clearScreen();
    int transaction = -1;
    std::cout << "\tTransaction Type" << std::endl;
    std::cout << "1. Withdrawl" << std::endl;
    std::cout << "2. Deposit" << std::endl;
    std::cout << "Select Transaction: " << std::endl;
    std::cin >> transaction;

    if (transaction == WITHDRAWL) {
        withdraw(accountNumber);
    }
    else {
        deposit(accountNumber);
    }

    std::cin.get();
}

void BankSystemManager::updateAccount() {
    int updateOption;
    std::string name;
    std::string accountNumber;

    //Request what to update
    std::cout << "\tUpdate Account Information" << std::endl;
    std::cout << "1. Update Phone Number" << std::endl;
    std::cout << "2. Update Home Address" << std::endl;
    std::cout << "Select information to update: ";
    std::cin >> updateOption;
    
    std::cout << "Enter Customer's Name: ";
    std::cin.ignore();
    std::getline(std::cin, name);

    //Lookup account and get account number if it exists
    accountNumber = this->dbHandler.getAccountNumber(name);
    if (accountNumber.empty()) {
        std::cout << "No account associated with the name " + name + " does not exist!" << std::endl;
        return;
    }

    //Update Account
    this->dbHandler.updateAccount(accountNumber, updateOption);

    //Update the date
    this->dbHandler.updateDate(getCurrentDate(), accountNumber);
}

int BankSystemManager::menu() {
    int userAction;

    //Display Menu with options
    printMainMenu();

    //Get option
    std::cout << "\nEnter your choice: ";
    std::cin >> userAction;

    clearScreen();
    return userAction;
}

void BankSystemManager::run() {
    int doExit = false;
    int option = INT_MAX;

    while (!doExit) {
        switch (menu()) {
        case ACTION::CREATE:
            createAccount();
            clearScreen();
            break;
        case ACTION::UPDATE:
            updateAccount();
            clearScreen();
            break;
        case ACTION::REMOVE:
            deleteAccount();
            clearScreen();
            break;
        case ACTION::DISPLAY:
            break;
        case ACTION::TRANSACTION:
            transaction();
            clearScreen();
            break;
        case ACTION::EXIT:
            std::cout << "Exiting the program. Goodbye!" << std::endl;
            doExit = true;
            break;
        }
    }
}