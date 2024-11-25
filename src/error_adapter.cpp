#include "database/error_adapter.hpp"
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/stdx.hpp>

using bsoncxx::builder::basic::make_document;
using bsoncxx::builder::basic::kvp;

void ErrorAdapter::insertError(const std::string& id, const std::string& robotID, const std::string& errorType, const int& resolved) {

    auto query_doc = make_document(kvp("_id", id));
    auto existing_doc = collection_.find_one(query_doc.view());
    if (existing_doc) {
        throw std::invalid_argument("The error document cannot be added to the database because of a duplicate ID.");
    } else if (!allErrorIsResolved(robotID)) {
        throw std::invalid_argument("The error document cannot be added to the database because there exists an error of the same robot that has not been resolved.");
    } else {
        auto error_doc = make_document(
            kvp("_id", id),
            kvp("robot_id", robotID),
            kvp("error_type", errorType),
            kvp("resolved", resolved)
        );
        collection_.insert_one(error_doc.view());
    }
}

bool ErrorAdapter::allErrorIsResolved(const std::string& robotID) {
    auto query_doc = make_document(kvp("robot_id", robotID), kvp("resolved", 0));
    auto unresolved_count = collection_.count_documents(query_doc.view());
    return unresolved_count == 0;
}

std::optional<bsoncxx::document::value> ErrorAdapter::findDocumentById(const std::string& errorId) {
    auto query_doc = make_document(kvp("_id", errorId));
    auto result = collection_.find_one(query_doc.view());

    if (result) {
        return std::optional<bsoncxx::document::value>{std::move(*result)};
    } else {
        return std::nullopt;
    }
}

std::vector<bsoncxx::document::value> ErrorAdapter::findErrorByRobotID(const std::string& robotId) {
    std::vector<bsoncxx::document::value> errorDocuments;
    auto query_doc = make_document(kvp("robot_id", robotId));

    // Find documents matching the query
    auto cursor = collection_.find(query_doc.view());
    if (cursor.begin() != cursor.end()) {
        // Add all matching documents to the result vector
        for (const auto& doc : cursor) {
            errorDocuments.emplace_back(bsoncxx::document::value(doc));
        }
    } else {
        std::cerr << "No errors found for robot ID " << robotId << std::endl;
    }
    return errorDocuments;
}

bool ErrorAdapter::updateError(const std::string& id, const std::string& robotID, const std::string& errorType, const int& resolved) {
    auto query_doc = make_document(kvp("_id", id));
    auto update_doc = make_document(
        kvp("$set",
            make_document(
                kvp("resolved", resolved)
            )
        )
        
    );

    auto result = collection_.update_one(query_doc.view(), update_doc.view());
    return result && result->modified_count() > 0;
}

bool ErrorAdapter::deleteError(const std::string& errorId) {
    auto query_doc = make_document(kvp("_id", errorId));
    auto result = collection_.delete_one(query_doc.view());
    return result && result->deleted_count() > 0;
}

std::vector<bsoncxx::document::value> ErrorAdapter::getAllErrors() {
    std::vector<bsoncxx::document::value> errors;
    auto cursor = collection_.find({});
    for (const auto& doc : cursor) {
        errors.emplace_back(bsoncxx::document::value(doc));
    }
    return errors;
}
