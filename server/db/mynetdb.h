#ifndef NETWORK_DB_H
#define NETWORK_DB_H

#include <string>
#include <vector>
#include <sqlite3.h>


class NetworkDB {
public:
    NetworkDB();
    ~NetworkDB();

    bool insertDataWithParams(const std::string& name, std::string& age);

private:
    const char *filename = "data_db/network.db";
    sqlite3* db;
    char* errMsg = nullptr;

    bool executeQuery(const std::string& sql);
    bool creataTable();
    
};

#endif // NETWORK_DB_H