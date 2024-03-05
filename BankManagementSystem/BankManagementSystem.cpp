#include "BankManagementSystem.h"

//Helper Methods
void BankSystemManager::pauseConsole() {
    std::cout << "Press Enter to continue...";
    std::cin.ignore();
    std::cin.get();
}

void BankSystemManager::clearScreen() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

void BankSystemManager::printMainMenu() {
    // Print the title
    printBanner("Banking System Options");

    // Print the options
    std::cout << "1. Create Account\n";
    std::cout << "2. Update Account\n";
    std::cout << "3. Remove Account\n";
    std::cout << "4. Display Account\n";
    std::cout << "5. Transaction\n";
    std::cout << "0. Exit\n";

    std::cout << std::setfill(' ');
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
    //Get current balance
    double balance;
    this->dbHandler.getData(accountNumber, BALANCE, &balance);
    std::cout << "\nCurrent Balance: " << balance << std::endl;

    //Prompt User for how much to withdraw
    double withdrawAmount = 0.00;
    std::cout << "Enter amount to withdraw: ";
    std::cin >> withdrawAmount;

    //If the amount to be withdrawn is greater then the current balance
    //Do nothing, cannot withdraw more than they have
    if (withdrawAmount > balance) {
        std::cout << "Insufficient Funds. Cannot withdraw $" << withdrawAmount << ". Balance: $" << balance << std::endl;
        return;
    }

    //Subtract withdraw amount from balance and update database
    balance -= withdrawAmount;

    if (balance < 0) {
        balance = 0;
    }

    std::cout << "Updated Balance: " << balance << std::endl;
    this->dbHandler.updateBalance(accountNumber, balance);
}

void BankSystemManager::deposit(std::string accountNumber) {
    //Get Current Balance
    double balance;
    this->dbHandler.getData(accountNumber, BALANCE, &balance);
    std::cout << "\nCurrent Balance: " << balance << std::endl;

    //Prompt User for deposit amount
    double depositAmount = 0.00;
    std::cout << "Enter amount to deposit: ";
    std::cin >> depositAmount;

    //Add deposit amount to current balance
    balance += depositAmount;

    //Update Database
    this->dbHandler.updateBalance(accountNumber, balance);
}

void BankSystemManager::printBanner(std::string title) {
    int totalWidth = 40; // Total width of the border
    int titleWidth = title.length(); // Length of the title

    // Calculate the left and right padding for the title to center it within the border
    int leftPadding = (totalWidth - titleWidth) / 2;
    int rightPadding = totalWidth - titleWidth - leftPadding;

    // Print the border with centered title
    std::cout << std::setw(totalWidth) << std::setfill('=') << '\n';
    std::cout << std::setw(leftPadding) << std::setfill(' ') << "" << title << std::setw(rightPadding) << std::setfill(' ') << "" << '\n';
    std::cout << std::setw(totalWidth) << std::setfill('=') << '\n';
    std::cout << std::setw(0) << std::right;
    std::cout << std::setfill(' ');
}

//Main Methods
BankSystemManager::BankSystemManager(const std::string& dbName): dbHandler(dbName) {
	//Initializes the Database Handler object, which attempts to create the
	//accounts database if it does not exist
	return;
}

void BankSystemManager::createAccount() {
    printBanner("Account Creation");

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
    printBanner("Delete Account");
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

    printBanner("Transaction");

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
        std::cout << "Enter Customer's Name: ";
        std::getline(std::cin, name);

        std::cout << name << std::endl;
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
    std::cout << "Select Transaction: ";
    std::cin >> transaction;

    if (transaction == WITHDRAWL) {
        withdraw(accountNumber);
    }
    else {
        deposit(accountNumber);
    }
}

void BankSystemManager::updateAccount() {
    int updateOption;
    std::string input;
    std::string accountNumber;

    // Define regular expressions for account number and full name
    std::regex accountNumberPattern(R"(\d{12})"); // Assuming account number is 9 digits
    std::regex fullNamePattern(R"([A-Za-z]+(\s[A-Za-z]+){1,3})"); // Allow up to 4 names

    //Request what to update
    printBanner("Update Account Information");
    std::cout << "1. Update Phone Number" << std::endl;
    std::cout << "2. Update Home Address" << std::endl;
    std::cout << "Select information to update: ";
    std::cin >> updateOption;
    
    std::cout << "Enter customer's name or account number: ";
    std::cin.ignore();
    std::getline(std::cin, input);

    // Check if the input matches the patterns
    if (std::regex_match(input, accountNumberPattern)) {
        accountNumber = input;
    }
    else if (std::regex_match(input, fullNamePattern)) {
        //Lookup account and get account number if it exists
        accountNumber = this->dbHandler.getAccountNumber(input);
        if (accountNumber.empty()) {
            std::cout << "No account associated with the name " + input + " does not exist!" << std::endl;
            return;
        }
    }
    else {
        std::cout << "Input does not match any known pattern." << std::endl;
    }

    //Update Account
    this->dbHandler.updateAccount(accountNumber, updateOption);

    //Update the date
    this->dbHandler.updateDate(getCurrentDate(), accountNumber);
}

void BankSystemManager::displayAccounts() {
    //Define if they want to display a specific account or all accounts
    const int allAccounts{ 1 };
    const int singleAccount{ 2 };
    int userOption = -1;

    printBanner("Display Options");
    std::cout << "1. Display All Accounts" << std::endl;
    std::cout << "2. Display Specific Account" << std::endl;
    std::cout << "Enter display option: ";
    std::cin >> userOption;

    if (userOption == allAccounts) {
        //Display all accounts in database
        clearScreen();
        std::cout << std::left << std::setw(20) << "Account Number";
        std::cout << std::setw(20) << "Name";
        std::cout << std::setw(20) << "Phone";
        std::cout << std::setw(20) << "AccountType" << std::endl;
        this->dbHandler.DisplayAccount();
    } else{
        //Display specific account
        std::string accountNumber;
        std::string name;
        std::string option;

        //Get customer's account number
        std::cout << "Does Customer know their account number: ";
        std::cin.ignore();
        std::getline(std::cin, option);
        
        if (option == "yes") {
            std::cout << "Enter accounter number: ";
            std::getline(std::cin, accountNumber);
        }
        else {
            std::cout << "Enter customer's name: ";
            std::getline(std::cin, name);
            accountNumber = this->dbHandler.getAccountNumber(name);
        }

        //Display Customer's data
        clearScreen();
        std::cout << std::left << std::setw(20) << "Account Number";
        std::cout << std::setw(15) << "Name";
        std::cout << std::setw(15) << "Phone";
        std::cout << std::setw(15) << "AccountType" << std::endl;
        this->dbHandler.DisplayAccount(accountNumber);
    }

    //Reset fill character and format
    std::cout << std::setw(0) << std::right;
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
            pauseConsole();
            clearScreen();
            break;
        case ACTION::UPDATE:
            updateAccount();
            pauseConsole();
            clearScreen();
            break;
        case ACTION::REMOVE:
            deleteAccount();
            pauseConsole();
            clearScreen();
            break;
        case ACTION::DISPLAY:
            displayAccounts();
            pauseConsole();
            clearScreen();
            break;
        case ACTION::TRANSACTION:
            transaction();
            pauseConsole();
            clearScreen();
            break;
        case ACTION::EXIT:
            std::cout << "Exiting the program. Goodbye!" << std::endl;
            doExit = true;
            break;
        }
    }
}