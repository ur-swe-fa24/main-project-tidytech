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
    auto collection = db["tasks"];

    // create taskAdapter
    TaskAdapter taskAdapter(collection);
    SECTION("Insert Task 1 and 2") {
        // insert a task and check if it is added
        taskAdapter.insertTask("1", "task1", "clean the floor1", "10:00", "13:00", "in progress", "50", {"1", "2", "3"});
        auto foundTask = taskAdapter.findDocumentById("1");
        REQUIRE(foundTask);  // Assert that the task exists
        REQUIRE(bsoncxx::to_json(*foundTask).find("task1") != std::string::npos); 
    }


}