#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/catch_approx.hpp>

#include <iostream>
#include "database/robot_adapter.hpp" 
#include "database/task_adapter.hpp" 
#include <bsoncxx/json.hpp>
#include "database/floor_adapter.hpp" 


mongocxx::instance instance{};
mongocxx::client client{mongocxx::uri{}};
auto db = client["test_database"];

/**
 * unit tests for robot adapter
 */
TEST_CASE("Robot Adapter Unit Tests") {
    auto db = client["test_database"];
    auto collection = db["robots"];
    RobotAdapter robotAdapter(collection);

    //testing inserting and finding robot
    SECTION("Insert and Find Robot") {
        robotAdapter.insertRobot("robot1", "medium", "typeA", "baseLocationA", "currentLocationA", "active");
        auto foundRobot = robotAdapter.findDocumentById("robot1");
        REQUIRE(foundRobot);
        REQUIRE(bsoncxx::to_json(*foundRobot).find("robot1") != std::string::npos);

        //exception when trying to insert a robot with a duplicate ID
        REQUIRE_THROWS(robotAdapter.insertRobot("robot1", "small", "typeB", "baseLocationB", "currentLocationB", "inactive"));
    }

    //testing updating robot location
    SECTION("Update Robot Location") {
        robotAdapter.updateRobotLocation("robot1", "newLocationA");
        auto foundRobot = robotAdapter.findDocumentById("robot1");
        REQUIRE(foundRobot);
        REQUIRE(bsoncxx::to_json(*foundRobot).find("newLocationA") != std::string::npos);
    }

    //testing update robot status
    SECTION("Update Robot Status") {
        robotAdapter.updateRobotStatus("robot1", "inactive");
        auto foundRobot = robotAdapter.findDocumentById("robot1");
        REQUIRE(foundRobot);
        REQUIRE(bsoncxx::to_json(*foundRobot).find("inactive") != std::string::npos);
    }

    //testing delete robot
    SECTION("Delete Robot") {
        robotAdapter.deleteRobot("robot1");
        auto foundRobot = robotAdapter.findDocumentById("robot1");
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
        floorAdapter.insertFloor("floor1", "Bedroom", "Carpet", "Small", "Medium", "false", "90");
        auto foundFloor = floorAdapter.findDocumentById("floor1");
        REQUIRE(foundFloor);
        REQUIRE(bsoncxx::to_json(*foundFloor).find("floor1") != std::string::npos);

        //exception when trying to insert a floor with a duplicate ID
        REQUIRE_THROWS(floorAdapter.insertFloor("floor1", "Living Room", "Wood", "Large", "High", "true", "85"));
    }

    //testing updating floor clean level
    SECTION("Update Floor Clean Level") {
        floorAdapter.updateCleanLevel("floor1", "75");
        auto foundFloor = floorAdapter.findDocumentById("floor1");
        REQUIRE(foundFloor);
        REQUIRE(bsoncxx::to_json(*foundFloor).find("75") != std::string::npos);
    }

    //testing updating floor restrictions
    SECTION("Update Floor Restriction") {
        floorAdapter.updateRestriction("floor1", "true");
        auto foundFloor = floorAdapter.findDocumentById("floor1");
        REQUIRE(foundFloor);
        REQUIRE(bsoncxx::to_json(*foundFloor).find("true") != std::string::npos);

    }

    //testing delete floor
    SECTION("Delete Floor") {
        floorAdapter.deleteFloor("floor1");
        auto foundFloor = floorAdapter.findDocumentById("floor1");
        REQUIRE(!foundFloor);
    }
}

/**
 * unit tests for task adapter
 */
TEST_CASE("Task Adapter Unit Tests") {

}