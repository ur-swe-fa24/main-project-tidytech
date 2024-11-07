#ifndef DATABASE_DB_Manager_HPP
#define DATABASE_DB_Manager_HPP

#pragma once
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/database.hpp>
#include <memory>
#include <string>

class DBManager {
public:
    static DBManager& getInstance(const std::string& uri, const std::string& dbName) {
        static DBManager instance(uri, dbName);
        return instance;
    }

    // Provide access to the database
    mongocxx::database getDatabase() const;

private:
    mongocxx::instance instance_;         // Ensures MongoDB driver is initialized once
    mongocxx::client client_;             // The MongoDB client instance
    mongocxx::database database_;         // The MongoDB database instance

    // Private constructor for singleton
    DBManager(const std::string& uri, const std::string& dbName)
        : client_(mongocxx::uri{uri}), database_(client_[dbName]) {}

    // Delete copy constructor and assignment operator
    DBManager(const DBManager&) = delete;
    DBManager& operator=(const DBManager&) = delete;
};

#endif