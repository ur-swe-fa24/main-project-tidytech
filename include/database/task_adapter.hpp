#ifndef DATABASE_TASK_ADAPTER_HPP
#define DATABASE_TASK_ADAPTER_HPP

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/document/value.hpp>
#include <mongocxx/collection.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <string>
#include <vector>

class TaskAdapter {
public:
    TaskAdapter(mongocxx::collection collection) : collection_(std::move(collection)) {}

    // create a new task document
    void insertTask(const std::string& id, const std::string& name, const std::string& description,
                    const std::string& start_time, const std::string& end_time, const std::string& status,
                    const std::string& completion, const std::vector<std::string>& assigned_robots);

    // find a document by task id
    std::optional<bsoncxx::document::value> findDocumentById(const std::string& taskId);

    // update a specific attribute of a task
    bool updateTaskStartTime(const std::string& taskId, const std::string& start_time);
    bool updateTaskEndTime(const std::string& taskId, const std::string& end_time);
    bool updateTaskStatus(const std::string& taskId, const std::string& status);
    bool updateTaskCompletion(const std::string& taskId, const std::string& completion);
    bool updateTaskRobotAssignment(const std::string& taskId, const std::vector<std::string>& assigned_robots);
    
    // delete a task by id
    bool deleteTask(const std::string& taskId);

private:
    mongocxx::collection collection_;
};

#endif