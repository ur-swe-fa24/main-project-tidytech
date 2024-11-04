#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/catch_approx.hpp>

#include <iostream>
#include "database/robot_adapter.hpp" 
#include "database/task_adapter.hpp" 
#include <bsoncxx/json.hpp>
#include "database/floor_adapter.hpp" 

/**
 * unit tests for robot adapter
 */
TEST_CASE("Robot Adapter Unit Tests") {

}

/**
 * unit tests for floor adapter
 */
TEST_CASE("Floor Adapter Unit Tests") {

}


/**
 * unit tests for task adapter
 */
TEST_CASE("Task Adapter Unit Tests") {
    // create the MongoDB instance
    mongocxx::instance instance{};
    mongocxx::client client{mongocxx::uri{}};
    
    // get the database and collection
    auto db = client["test_database"];
    db = client["test_database"];
    auto collection = db["tasks"];

    // create taskAdapter
    TaskAdapter taskAdapter(collection);
    SECTION("Insert Task 1") {
        taskAdapter.deleteTask("1"); // delete the task first if it exists to ensure clean state
        // insert a task and check if it is added
        taskAdapter.insertTask("1", "task1", "clean floor 1", "10:00", "13:00", "in progress", "50", {"1", "2", "3"});
        auto foundTask = taskAdapter.findDocumentById("1");
        REQUIRE(foundTask);  // Assert that the task exists
        REQUIRE(bsoncxx::to_json(*foundTask).find("task1") != std::string::npos); 
    }

    SECTION("Duplicated ids cannot be inserted to the database") {
        REQUIRE_THROWS(taskAdapter.insertTask("1", "task1", "clean the floor2", "11:00", "13:00", "in progress", "50", {"1", "2", "3"}));
    }

    SECTION("Find a task by id") {
        taskAdapter.deleteTask("2");
        taskAdapter.insertTask("2", "task2", "clean the bathroom2", "14:00", "14:20", "pending", "0", {"1"});
        auto foundTask = taskAdapter.findDocumentById("2");
        REQUIRE(foundTask);
        REQUIRE(bsoncxx::to_json(*foundTask).find("task2") != std::string::npos);
    }

    SECTION("Update Task Status") {
        taskAdapter.deleteTask("3");
        // insert a task, update its status, and check the update
        taskAdapter.insertTask("3", "task3", "clean the second floor", "15:00", "16:00", "pending", "10", {"2"});
        taskAdapter.updateTaskStatus("3", "completed");
        auto updatedTask = taskAdapter.findDocumentById("3");
        REQUIRE(updatedTask); 
        // check if status updated
        REQUIRE(bsoncxx::to_json(*updatedTask).find("completed") != std::string::npos);  
    }

    SECTION("Update Task Robot Assignment") {
        taskAdapter.deleteTask("4");
        // insert a task, update its robot assignment, and check the change
        taskAdapter.insertTask("4", "task4", "clean the 3rd floor", "17:00", "18:30", "in progress", "20", {"1"});
        taskAdapter.updateTaskRobotAssignment("4", {"3", "4", "5"});
        auto updatedTask = taskAdapter.findDocumentById("4");
        REQUIRE(updatedTask);
        // check if robot "3" is assigned
        REQUIRE(bsoncxx::to_json(*updatedTask).find("3") != std::string::npos);  
    }

}