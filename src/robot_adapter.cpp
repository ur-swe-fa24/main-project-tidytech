#include "database/robot_adapter.hpp"
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/stdx.hpp>

using bsoncxx::builder::basic::make_document;
using bsoncxx::builder::basic::kvp;

void RobotAdapter::insertRobot(const std::string& id, const std::string& name, const std::string& size, const std::string& type,
                                const std::string& baseLocation, const std::string& currentLocation,
                                const std::string& status) {

    auto query_doc = bsoncxx::builder::basic::make_document(
        bsoncxx::builder::basic::kvp("_id", id)
    );
    auto existing_doc = collection_.find_one(query_doc.view());
    if (!existing_doc) {
        auto robot_doc = make_document(
        kvp("_id", id),
        kvp("name", name),
        kvp("size", size),
        kvp("type", type),
        kvp("baseLocation", baseLocation),
        kvp("currentLocation", currentLocation),
        kvp("status", status));
    // insert the doc into the collection
        collection_.insert_one(robot_doc.view());
    } else {
        std::cout << "the robot has been added to the database already" << std::endl;
    }
}

std::optional<bsoncxx::document::value> RobotAdapter::findDocumentById(const std::string& robotId) {
    // build the query doc
    auto query_doc = bsoncxx::builder::basic::make_document(
        bsoncxx::builder::basic::kvp("_id", robotId)
    );
    
    mongocxx::stdx::optional<bsoncxx::document::value> result = collection_.find_one(query_doc.view());
    
    // Check if the result contains a document
    if (result) {
        return std::optional<bsoncxx::document::value>{std::move(*result)};
    } else {
        return std::nullopt;
    }
}

bool RobotAdapter::updateRobotStatus(const std::string& robotId, const std::string& newStatus) {
    auto query_doc = make_document(
        kvp("_id", robotId)
    );

    auto update_doc = make_document(
        kvp("$set", 
            make_document(
                kvp("status", newStatus)
            )
        )
    );

    auto result = collection_.update_one(query_doc.view(), update_doc.view());
    return result && result->modified_count() > 0;
}

bool RobotAdapter::updateRobotLocation(const std::string& robotId, const std::string& newLocation) {
    // build the query document to find the robot by ID
    auto query_doc = bsoncxx::builder::basic::make_document(
        bsoncxx::builder::basic::kvp("_id", robotId)
    );

    // build the update document for the current location
    auto update_doc = make_document(
        kvp("$set", 
            make_document(
                kvp("currentLocation", newLocation)
            )
        )
    );

    // execute the update operation
    auto result = collection_.update_one(query_doc.view(), update_doc.view());
    return result && result->modified_count() > 0; 
}

bool RobotAdapter::deleteRobot(const std::string& robotId) {
    auto query_doc = make_document(
        kvp("_id", robotId)
    );

    auto result = collection_.delete_one(query_doc.view());
    return result && result->deleted_count() > 0;
}
