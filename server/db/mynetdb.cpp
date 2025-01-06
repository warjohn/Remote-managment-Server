#include <iostream>
#include <sqlite3.h>
#include "mynetdb.h"



NetworkDB::NetworkDB() {
    if (sqlite3_open(filename, &db) != SQLITE_OK) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
    }
    //std::cout << "Database opened successfully!" << std::endl;
}

NetworkDB::~NetworkDB() {
    if (db) {
        sqlite3_close(db);
        //std::cout << "Database closed." << std::endl;
    }
}

bool NetworkDB::executeQuery(const std::string& sql) {
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool NetworkDB::creataTable() {
    const std::string createTableSQL = 
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "IP_adress TEXT NOT NULL, "
        "connection_time INTEGER NOT NULL);";

    return executeQuery(createTableSQL);
}


bool NetworkDB::insertDataWithParams(const std::string& name, std::string& age) {

    if (!creataTable()) {
        std::cout << "Table created is not successfully! Something went wronge" << std::endl;
    }

    const std::string insertSQL = "INSERT INTO users (IP_adress, connection_time) VALUES (?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, insertSQL.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Привязка параметров
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, age.c_str(), -1, SQLITE_STATIC);
    
    // Выполнение запроса
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error executing statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}
