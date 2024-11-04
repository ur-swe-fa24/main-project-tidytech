#include <iostream>
#include "database/robot_adapter.hpp" 
#include "database/task_adapter.hpp" 
#include <bsoncxx/json.hpp>
#include "database/floor_adapter.hpp" 

int main() {
    // Initialize MongoDB instance
    mongocxx::instance instance{};
    mongocxx::client client{mongocxx::uri{}};

    // Create or get a database and collection
    auto db = client["test_database"];
    auto collection = db["robots"];

    // Initialize RobotAdapter
    RobotAdapter robotAdapter(collection);

    // Test inserting a robot
    robotAdapter.insertRobot("robot1", "medium", "typeA", "baseLocationA", "currentLocationA", "active");
    std::cout << "Inserted robot1." << std::endl;

    // Test finding the robot
    auto foundRobot = robotAdapter.findDocumentById("robot1");
    if (foundRobot) {
        std::cout << "Found robot1: " << bsoncxx::to_json(*foundRobot) << std::endl;
    } else {
        std::cout << "robot1 not found." << std::endl;
    }

    // Test updating the robot's location
    robotAdapter.updateRobotLocation("robot1", "newLocationA");
    foundRobot = robotAdapter.findDocumentById("robot1");
    if (foundRobot) {
        std::cout << "Updated robot1: " << bsoncxx::to_json(*foundRobot) << std::endl;
    }

    // Test updating the robot's status
    robotAdapter.updateRobotStatus("robot1", "inactive");
    foundRobot = robotAdapter.findDocumentById("robot1");
    if (foundRobot) {
        std::cout << "Updated robot1 status: " << bsoncxx::to_json(*foundRobot) << std::endl;
    }

    // Test deleting the robot
    robotAdapter.deleteRobot("robot1");
    foundRobot = robotAdapter.findDocumentById("robot1");
    if (foundRobot) {
        std::cout << "robot1 found after deletion: " << bsoncxx::to_json(*foundRobot) << std::endl;
    } else {
        std::cout << "robot1 successfully deleted." << std::endl;
    }

    // Test 2: taskAdapter
    collection = db["tasks"];

    // Initialize taskAdapter
    TaskAdapter taskAdapter(collection);

    // Test inserting a task
    taskAdapter.insertTask("1", "task1", "clean the floor1", "10:00", "13:00", "in progress", "50", {"1", "2", "3"});
    std::cout << "Inserted task1." << std::endl;

    taskAdapter.insertTask("2", "task2", "clean the bathroom2", "14:00", "14:20", "pending", "0", {"1"});
    std::cout << "Inserted task2." << std::endl;

    // Test finding the task
    auto foundTask = taskAdapter.findDocumentById("1");
    if (foundTask) {
        std::cout << "Found task1: " << bsoncxx::to_json(*foundTask) << std::endl;
    } else {
        std::cout << "task1 not found." << std::endl;
    }

    // Test updates methods:
    taskAdapter.updateTaskStatus("2", "in progress");
    foundTask = taskAdapter.findDocumentById("2");
    if (foundTask) {
        std::cout << "Updated task 2 status: " << bsoncxx::to_json(*foundTask) << std::endl;
    }

    // update the task assignment
    taskAdapter.updateTaskRobotAssignment("1", {"2", "3", "4"});
    foundTask = robotAdapter.findDocumentById("1");
    if (foundTask) {
        std::cout << "Updated robot1 status: " << bsoncxx::to_json(*foundTask) << std::endl;
    }

    // Test deleting the task
    taskAdapter.deleteTask("1");
    foundTask = taskAdapter.findDocumentById("1");
    if (foundTask) {
        std::cout << "task1 found after deletion: " << bsoncxx::to_json(*foundTask) << std::endl;
    } else {
        std::cout << "task1 successfully deleted." << std::endl;

    // Test 3: FloorAdapter
    // Create a collection
    collection = db["floors"];

    // Initialize FloorAdapter
    FloorAdapter floorAdapter(collection);

    //floor test insert
    floorAdapter.insertFloor("floor1", "Bedroom", "Carpet", "Small", "Medium", "false", "90");
    std::cout << "Inserted floor1." << std::endl;

    // Test finding the floor
    auto foundFloor = floorAdapter.findDocumentById("floor1");
    if (foundFloor) {
        std::cout << "Found floor1: " << bsoncxx::to_json(*foundFloor) << std::endl;
    } else {
        std::cout << "floor1 not found." << std::endl;
    }

    //test updating clean level
    floorAdapter.updateCleanLevel("floor1", "75");
    foundFloor = floorAdapter.findDocumentById("floor1");
    if (foundFloor) {
        std::cout << "Updated floor1 clean level: " << bsoncxx::to_json(*foundFloor) << std::endl;
    }
    
    //update robot restrictions
    floorAdapter.updateRestriction("floor1", "true");
    foundFloor = floorAdapter.findDocumentById("floor1");
    if (foundFloor) {
        std::cout << "Updated floor1 restriction: " << bsoncxx::to_json(*foundFloor) << std::endl;
    }

    //delete floor
    floorAdapter.deleteFloor("floor1");
    foundFloor = floorAdapter.findDocumentById("floor1");
    if (foundFloor) {
        std::cout << "floor1 found after deletion: " << bsoncxx::to_json(*foundFloor) << std::endl;
    } else {
        std::cout << "floor1 successfully deleted." << std::endl;
    }

    return 0;
}


