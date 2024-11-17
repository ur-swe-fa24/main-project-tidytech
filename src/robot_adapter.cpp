#include "database/robot_adapter.hpp"
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/stdx.hpp>

using bsoncxx::builder::basic::make_document;
using bsoncxx::builder::basic::kvp;

void RobotAdapter::insertRobot(const std::string& id, const std::string& name, const std::string& size, const std::string& type,
                                const std::string& baseLocation, const std::string& currentLocation,
                                const std::string& status, const std::string& capacity) {

    auto query_doc = bsoncxx::builder::basic::make_document(
        bsoncxx::builder::basic::kvp("_id", id)
    );
    auto query_name = bsoncxx::builder::basic::make_document(
        bsoncxx::builder::basic::kvp("name", name)
    );
    auto existing_doc = collection_.find_one(query_doc.view());
    auto existing_name = collection_.find_one(query_name.view());
    if (existing_doc || name == "") {
        //throw error
        throw std::invalid_argument("the robot cannot be added to database because of duplicate id");
    } 
    //if the name is already in the database throw error
    else if (existing_name){
        throw std::invalid_argument("the robot cannot be added to database because of duplicate name");
    } else {
        auto robot_doc = make_document(
            kvp("_id", id),
            kvp("name", name),
            kvp("size", size),
            kvp("type", type),
            kvp("base_location", baseLocation),
            kvp("current_location", currentLocation),
            kvp("status", status),
            kvp("capacity", capacity));
        collection_.insert_one(robot_doc.view());
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
                kvp("current_location", newLocation)
            )
        )
    );

    // execute the update operation
    auto result = collection_.update_one(query_doc.view(), update_doc.view());
    return result && result->modified_count() > 0; 
}


bool RobotAdapter::updateRobotCapacity(const std::string& robotId, const std::string& newCapacity) {
    // build the query document to find the robot by ID
    auto query_doc = bsoncxx::builder::basic::make_document(
        bsoncxx::builder::basic::kvp("_id", robotId)
    );

    // build the update document for the current capacity
    auto update_doc = make_document(
        kvp("$set", 
            make_document(
                kvp("capacity", newCapacity)
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

std::vector<bsoncxx::document::value> RobotAdapter::getAllRobots() {
    std::vector<bsoncxx::document::value> robots;
    try {
        auto cursor = collection_.find({});
        for (const auto& doc : cursor) {
            robots.emplace_back(bsoncxx::document::value(doc));
        }
    } catch (const std::exception& e) {
        std::cerr << "Error fetching all robots: " << e.what() << std::endl;
    }
    return robots;
}
