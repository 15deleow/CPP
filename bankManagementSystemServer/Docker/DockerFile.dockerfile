# Base image with necessary dependencies for compiling C++ code
FROM ubuntu:latest

# Install build tools and required libraries
RUN apt-get update && apt-get install -y \
    build-essential \
    g++

# Set working directory
WORKDIR /app

# Copy source files for Database Handler Class and Bank Manager Class
COPY database_handler /app/database_handler/
COPY BankManager /app/BankManager/
COPY Utilities /app/Utilities/

# Compile Database Handler Class
WORKDIR /app/DatabaseHandler/
RUN g++ -o databasehandler.o -c DatabaseHandler.cpp -I../Utilities

# Compile Bank Manager Class
WORKDIR /app/bank_manager/
RUN g++ -o BankManager.o -c BankManager.cpp -I ../database_handler -I../Utilities

# Link Bank Manager Class with Database Handler Class and create executable
RUN g++ -o bank_manager_server ../database_handler/DatabaseHandler.o BankManager.o

# Expose port for REST API
EXPOSE 8080

# Command to run Bank Manager server
CMD ["./bank_manager_server"]
