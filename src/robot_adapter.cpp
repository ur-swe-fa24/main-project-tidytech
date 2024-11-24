#include "database/robot_adapter.hpp"
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/stdx.hpp>

using bsoncxx::builder::basic::make_document;
using bsoncxx::builder::basic::kvp;

void RobotAdapter::insertRobot(const std::string& id, const std::string& name, const std::string& size, const std::string& type,
                                const std::string& baseLocation, const std::string& currentLocation,
                                const std::string& status, const std::string& capacity, const std::vector<int>& taskQueue, const std::vector<int>& path,
                                const int& totalBatteryUsed, const int& errorCount, const int& roomsCleaned) {

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
        bsoncxx::builder::basic::array task_queue;
        for (const auto& task : taskQueue) {
            task_queue.append(task);
        };
        bsoncxx::builder::basic::array robot_path;
        for (const auto& step : path) {
            robot_path.append(step);
        };
        auto robot_doc = make_document(
            kvp("_id", id),
            kvp("name", name),
            kvp("size", size),
            kvp("type", type),
            kvp("base_location", baseLocation),
            kvp("current_location", currentLocation),
            kvp("status", status),
            kvp("capacity", capacity),
            kvp("task_queue", task_queue),
            kvp("path", robot_path),
            kvp("total_battery_used", totalBatteryUsed),
            kvp("error_count", errorCount),
            kvp("rooms_cleaned", roomsCleaned));
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

/*
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
*/

bool RobotAdapter::updateRobot(const std::string& id, const std::string& currentLocation, const std::string& status, const std::string& capacity, 
                    const std::vector<int>& taskQueue, const std::vector<int>& path, const int& battery_changed) {
    auto query_doc = make_document(
        kvp("_id", id)
    );
    bsoncxx::builder::basic::array task_queue;
    for (const auto& task : taskQueue) {
        task_queue.append(task);
    };
    bsoncxx::builder::basic::array robot_path;
    for (const auto& step : path) {
        robot_path.append(step);
    };
    auto update_doc = make_document(
        kvp("$set", 
            make_document(
                kvp("status", status),
                kvp("current_location", currentLocation),
                kvp("capacity", capacity),
                kvp("task_queue", task_queue),
                kvp("path", robot_path)
            )
        ),
        kvp("$inc", 
            make_document(
                kvp("total_battery_used", battery_changed)
            )
        )
    );
    auto result = collection_.update_one(query_doc.view(), update_doc.view());
    return result && result->modified_count() > 0;
}


bool RobotAdapter::updateRobotErrorCount(const std::string& id) {
    auto query_doc = bsoncxx::builder::basic::make_document(
        bsoncxx::builder::basic::kvp("_id", id)
    );

    auto update_doc = make_document(
        kvp("$inc", 
            make_document(
                kvp("error_count", 1)
            )
        )
    );
    auto result = collection_.update_one(query_doc.view(), update_doc.view());
    return result && result->modified_count() > 0; 
}

bool RobotAdapter::updateRobotRoomsCleaned(const std::string& id) {
    auto query_doc = bsoncxx::builder::basic::make_document(
        bsoncxx::builder::basic::kvp("_id", id)
    );

    auto update_doc = make_document(
        kvp("$inc", 
            make_document(
                kvp("rooms_cleaned", 1)
            )
        )
    );
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
