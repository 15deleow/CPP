#include <iostream>
#include <mysql-cppconn-8/mysql/jdbc.h>

int main(void){
    try{
        sql::Driver * driver;
        sql::Connection * con;
        sql::Statement *stmt;
        sql::ResultSet *res;

        //Create a MySQL Connector/C++ Driver
        driver = get_driver_instance();

        //Connect to the MySQL server(MySQL container)
        con = driver->connect(
            "tcp://127.0.0.1:3306", "root", "sql_password23");

        //Create a test database
        stmt = con->createStatement();
        stmt->execute("CREATE DATABASE IF NOT EXISTS test_db");
        delete stmt;

        //Use the test database
        con->setSchema("test_db");

        //Create a table and insert some data
        stmt = con->createStatement();
        stmt->execute("CREATE TABLE IF NOT EXISTS test_table (id INT, name VARCHAR(50))");
        stmt->execute("INSERT INTO test_table VALUES(1, 'John')");
        stmt->execute("INSERT INTO test_table VALUES(2, 'Alice')");
        delete stmt;

        std::cout << "Data inserted into test_table." << std::endl;

        //Display the contentes of the data
        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT * FROM test_table");
        std::cout << "Table Contents: " << std::endl;
        while(res->next()){
            std::cout << "ID: " << res->getInt("id") << ", Name: ";
            std::cout << res->getString("name") << std::endl;
        }
        delete res;
        delete stmt;

        //Prompt the user to select a row
        int selectedId;
        std::cout << "Enter the ID of the row you want to select: " << std::endl;
        std::cin >> selectedId;

        //Fetch the selected row from the table
        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT * FROM test_table where id = " + std::to_string(selectedId));
        if (res->next()) {
            std::cout << "Selected Row: ID = " << res->getInt("id") << ", Name = " << res->getString("name") << std::endl;
        } else {
            std::cout << "Row with ID " << selectedId << " not found." << std::endl;
        }
        delete res;
        delete stmt;

        //Prompt user to continue
        std::cout << "Please hit enter to continue...";
        std::cin.ignore();
        std::cin.get();

        //Delete test database
        stmt = con->createStatement();
        stmt->execute("DROP DATABASE IF EXISTS test_db");
        delete stmt;

        std::cout << "Test database deleted." << std::endl;
        delete con;
        

    }catch(sql::SQLException &e){
        std::cout << "SQL Exception: " << e.what() << std::endl;
    }

    std::cout << "All Done!!" << std::endl;

    return 0;
}

