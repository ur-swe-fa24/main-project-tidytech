#include <iostream>
#include "database/robot_adapter.hpp" // Include your RobotAdapter header file
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

    // Create a collection
    auto collection2 = db["floors"];

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


