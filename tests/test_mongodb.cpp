#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/catch_approx.hpp>

#include <iostream>
#include "database/robot_adapter.hpp" 
#include "database/task_adapter.hpp" 
#include <bsoncxx/json.hpp>
#include "database/floor_adapter.hpp" 

// create the MongoDB instance
mongocxx::instance instance{};
mongocxx::client client{mongocxx::uri{}};

/**
 * unit tests for robot adapter
 */
TEST_CASE("Robot Adapter Unit Tests") {
    auto db = client["test_database"];
    auto collection = db["robots"];
    RobotAdapter robotAdapter(collection);

    //testing inserting and finding robot
    SECTION("Insert and Find Robot") {
        robotAdapter.insertRobot("1", "robot1", "medium", "typeA", "baseLocationA", "currentLocationA", "active", "100", {1, 2, 3}, {1, 2, 3}, 0, 0, 0);
        auto foundRobot = robotAdapter.findDocumentById("1");
        REQUIRE(foundRobot);
        REQUIRE(bsoncxx::to_json(*foundRobot).find("1") != std::string::npos);

        //exception when trying to insert a robot with a duplicate ID
        REQUIRE_THROWS(robotAdapter.insertRobot("1", "robot1","small", "typeB", "baseLocationB", "currentLocationB", "inactive", "100", {1, 2, 3}, {1, 2, 3}, 0, 0, 0));
    }

    //testing updating robot
    SECTION("Update Robot") {
        robotAdapter.updateRobot("1", "office2", "active", "100", {1, 2, 3}, {1, 2, 3}, 20, 0, 1);
        auto foundRobot = robotAdapter.findDocumentById("1");
        REQUIRE(foundRobot);
        REQUIRE(bsoncxx::to_json(*foundRobot).find("office2") != std::string::npos);
        REQUIRE(bsoncxx::to_json(*foundRobot).find("active") != std::string::npos);
    }

    // //testing update robot status
    // SECTION("Update Robot Status") {
    //     robotAdapter.updateRobotStatus("1", "inactive");
    //     auto foundRobot = robotAdapter.findDocumentById("1");
    //     REQUIRE(foundRobot);
    //     REQUIRE(bsoncxx::to_json(*foundRobot).find("inactive") != std::string::npos);
    // }

    //testing delete robot
    SECTION("Delete Robot") {
        robotAdapter.deleteRobot("1");
        auto foundRobot = robotAdapter.findDocumentById("1");
        REQUIRE(!foundRobot);
    }
}

/**
 * unit tests for floor adapter
 */
TEST_CASE("Floor Adapter Unit Tests") {
    auto db = client["test_database"];
    auto collection = db["floors"];
    FloorAdapter floorAdapter(collection);
    
    //testing inserting and finding floor
    SECTION("Insert and Find Floor") {
        floorAdapter.insertFloor("1", "floor1", "elevator", "Carpet", "Small", "Medium", "false", "90", {1,2});
        auto foundFloor = floorAdapter.findDocumentById("1");
        REQUIRE(foundFloor);
        REQUIRE(bsoncxx::to_json(*foundFloor).find("1") != std::string::npos);

        //exception when trying to insert a floor with a duplicate ID
        REQUIRE_THROWS(floorAdapter.insertFloor("1", "floor1", "hallway", "Wood", "Large", "High", "true", "85", {1,3}));
    }

    //testing updating floor clean level
    SECTION("Update Floor Clean Level") {
        floorAdapter.updateCleanLevel("1", "75");
        auto foundFloor = floorAdapter.findDocumentById("1");
        REQUIRE(foundFloor);
        REQUIRE(bsoncxx::to_json(*foundFloor).find("75") != std::string::npos);
    }

    //testing updating floor restrictions
    SECTION("Update Floor Restriction") {
        floorAdapter.updateRestriction("1", "true");
        auto foundFloor = floorAdapter.findDocumentById("1");
        REQUIRE(foundFloor);
        REQUIRE(bsoncxx::to_json(*foundFloor).find("true") != std::string::npos);

    }

    //testing delete floor
    SECTION("Delete Floor") {
        floorAdapter.deleteFloor("1");
        auto foundFloor = floorAdapter.findDocumentById("1");
        REQUIRE(!foundFloor);
    }
}


/**
 * unit tests for task adapter
 */
TEST_CASE("Task Adapter Unit Tests") {
    
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

    SECTION("Delete Task") {
        taskAdapter.insertTask("5", "task5", "clean the office", "11:00", "12:30", "in progress", "30", {"2"});
        taskAdapter.deleteTask("5");
        auto foundTask = taskAdapter.findDocumentById("5");
        REQUIRE(!foundTask);
    }

    SECTION("Delete all tasks") {
        taskAdapter.deleteTask("4");
        auto foundTask = taskAdapter.findDocumentById("4");
        REQUIRE(!foundTask);
        taskAdapter.deleteTask("3");
        foundTask = taskAdapter.findDocumentById("3");
        REQUIRE(!foundTask);
        taskAdapter.deleteTask("2");
        foundTask = taskAdapter.findDocumentById("2");
        REQUIRE(!foundTask);
        taskAdapter.deleteTask("1");
        foundTask = taskAdapter.findDocumentById("1");
        REQUIRE(!foundTask);
    }

}