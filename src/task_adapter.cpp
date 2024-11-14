#include "database/task_adapter.hpp"
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/stdx.hpp>
#include <stdexcept>

using bsoncxx::builder::basic::make_document;
using bsoncxx::builder::basic::kvp;

void TaskAdapter::insertTask(const std::string& id, const std::string& name, const std::string& description,
                            const std::string& start_time, const std::string& end_time, const std::string& status,
                            const std::string& completion, const std::vector<std::string>& assigned_robots) {
    // check if the task has already been inserted to the database
    auto query_doc = bsoncxx::builder::basic::make_document(
        bsoncxx::builder::basic::kvp("_id", id)
    );
    auto existing_doc = collection_.find_one(query_doc.view());
    if (!existing_doc || name = "") {
        bsoncxx::builder::basic::array assigned_robots_array;
        for (const auto& robot_id : assigned_robots) {
            assigned_robots_array.append(robot_id);
        };
        auto task_doc = make_document(
            kvp("_id", id),
            kvp("name", name),
            kvp("description", description),
            kvp("start_time", start_time),
            kvp("end_time", end_time),
            kvp("status", status),
            kvp("completion", completion),
            kvp("assigned_robots", assigned_robots_array));

        // insert the doc into the collection
        collection_.insert_one(task_doc.view());
    } else {
        throw std::invalid_argument("the task has been added to the database already" );
    }
}

std::optional<bsoncxx::document::value> TaskAdapter::findDocumentById(const std::string& taskId) {
    // build the query doc
    auto query_doc = bsoncxx::builder::basic::make_document(
        bsoncxx::builder::basic::kvp("_id", taskId)
    );
    
    mongocxx::stdx::optional<bsoncxx::document::value> result = collection_.find_one(query_doc.view());
    
    // check if the result contains a document
    if (result) {
        return std::optional<bsoncxx::document::value>{std::move(*result)};
    } else {
        return std::nullopt;
    }
}

bool TaskAdapter::updateTaskStartTime(const std::string& taskId, const std::string& newStartTime) {
    auto query_doc = bsoncxx::builder::basic::make_document(
        bsoncxx::builder::basic::kvp("_id", taskId)
    );

    // build the update document for the current location
    auto update_doc = make_document(
        kvp("$set", 
            make_document(
                kvp("start_time", newStartTime)
            )
        )
    );

    // execute the update operation
    auto result = collection_.update_one(query_doc.view(), update_doc.view());
    return result && result->modified_count() > 0; 
}

bool TaskAdapter::updateTaskEndTime(const std::string& taskId, const std::string& newEndTime) {
    auto query_doc = bsoncxx::builder::basic::make_document(
        bsoncxx::builder::basic::kvp("_id", taskId)
    );

    // build the update document for the current location
    auto update_doc = make_document(
        kvp("$set", 
            make_document(
                kvp("end_time", newEndTime)
            )
        )
    );

    // execute the update operation
    auto result = collection_.update_one(query_doc.view(), update_doc.view());
    return result && result->modified_count() > 0; 
}


bool TaskAdapter::updateTaskStatus(const std::string& taskId, const std::string& newStatus) {
    auto query_doc = make_document(
        kvp("_id", taskId)
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

bool TaskAdapter::updateTaskCompletion(const std::string& taskId, const std::string& newCompletion) {
    auto query_doc = make_document(
        kvp("_id", taskId)
    );

    auto update_doc = make_document(
        kvp("$set", 
            make_document(
                kvp("completion", newCompletion)
            )
        )
    );

    auto result = collection_.update_one(query_doc.view(), update_doc.view());
    return result && result->modified_count() > 0;
}

bool TaskAdapter::updateTaskRobotAssignment(const std::string& taskId, const std::vector<std::string>& newAssignedRobots) {
    auto query_doc = make_document(
        kvp("_id", taskId)
    );
    bsoncxx::builder::basic::array assigned_robots_array;
    for (const auto& robot_id : newAssignedRobots) {
        assigned_robots_array.append(robot_id);
    };

    auto update_doc = make_document(
        kvp("$set", 
            make_document(
                kvp("assigned_robots", assigned_robots_array)
            )
        )
    );

    auto result = collection_.update_one(query_doc.view(), update_doc.view());
    return result && result->modified_count() > 0;
}

bool TaskAdapter::deleteTask(const std::string& taskId) {
    auto query_doc = make_document(
        kvp("_id", taskId)
    );

    auto result = collection_.delete_one(query_doc.view());
    return result && result->deleted_count() > 0;
}