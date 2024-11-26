#ifndef DATABASE_ERROR_ADAPTER_HPP
#define DATABASE_ERROR_ADAPTER_HPP

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/document/value.hpp>
#include <mongocxx/collection.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <string>


class ErrorAdapter {
public:
    // Constructor to initialize the collection
    ErrorAdapter(mongocxx::collection collection) : collection_(std::move(collection)) {}

    // Insert a new error document
    void insertError(const std::string& id, const std::string& robotID, const std::string& errorType, const int& resolved);

    // Find a document by error ID
    std::optional<bsoncxx::document::value> findDocumentById(const std::string& errorId);
    std::vector<bsoncxx::document::value> findErrorByRobotID(const std::string& robotId);
    bool updateError(const std::string& id, const std::string& robotID, const std::string& errorType, const int& resolved);
    bool deleteError(const std::string& errorId);

    std::vector<bsoncxx::document::value> getAllErrors();
    bool allErrorIsResolved(const std::string& robotID);
    void resolveError(const std::string& robotID);

private:
    mongocxx::collection collection_;
};

#endif
