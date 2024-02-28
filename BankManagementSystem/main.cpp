// BankManagementSystem.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "BankManagementSystem.h"

#define databaseName "C:\\Users\\willi\\Documents\\CPP\\databases\\bankmanagementsystem.db"

int main()
{
    BankSystemManager mainSystem(databaseName);
            
    mainSystem.run();

    return 0;
}
