#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/catch_approx.hpp>

#include <iostream>
#include "database/robot_adapter.hpp" 
#include <bsoncxx/json.hpp>
#include "database/floor_adapter.hpp" 
#include "database/error_adapter.hpp" 

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
        robotAdapter.insertRobot("1", "robot1", "medium", "typeA", "baseLocationA", "currentLocationA", "active", "100", {1, 2, 3}, {1, 2, 3}, 100, 10, 0, 0);
        auto foundRobot = robotAdapter.findDocumentById("1");
        REQUIRE(foundRobot);
        REQUIRE(bsoncxx::to_json(*foundRobot).find("1") != std::string::npos);

        //exception when trying to insert a robot with a duplicate ID
        REQUIRE_THROWS(robotAdapter.insertRobot("1", "robot1","small", "typeB", "baseLocationB", "currentLocationB", "inactive", "100", {1, 2, 3}, {1, 2, 3}, 100, 0, 0, 0));
    }

    //testing updating robot
    SECTION("Update Robot") {
        robotAdapter.updateRobot("1", "office2", "active", "100", {1, 2, 3}, {1, 2, 3}, 80, 20);
        auto foundRobot = robotAdapter.findDocumentById("1");
        REQUIRE(foundRobot);
        REQUIRE(bsoncxx::to_json(*foundRobot).find("office2") != std::string::npos);
        REQUIRE(bsoncxx::to_json(*foundRobot).find("active") != std::string::npos);
        auto updatedTotalBatteryUsed = foundRobot->view()["total_battery_used"].get_int32().value;
        REQUIRE(updatedTotalBatteryUsed == 30);
    }

    //testing updating robot error count
    SECTION("Update Robot Error Count") {
        robotAdapter.updateRobotErrorCount("1");
        auto foundRobot = robotAdapter.findDocumentById("1");
        REQUIRE(foundRobot);
        auto updatedErrorCount = foundRobot->view()["error_count"].get_int32().value;
        REQUIRE(updatedErrorCount == 1);
    }

    //testing updating robot rooms cleaned count
    SECTION("Update Robot Rooms_cleaned count") {
        robotAdapter.updateRobotRoomsCleaned("1");
        auto foundRobot = robotAdapter.findDocumentById("1");
        REQUIRE(foundRobot);
        auto updatedRoomsCleanedCount = foundRobot->view()["rooms_cleaned"].get_int32().value;
        REQUIRE(updatedRoomsCleanedCount == 1);
    }

    //testing delete robot
    SECTION("Delete Robot") {
        robotAdapter.deleteRobot("1");
        auto foundRobot = robotAdapter.findDocumentById("1");
        REQUIRE(!foundRobot);
    }

    //testing getting all robots
    SECTION("Get All Robots") {
        robotAdapter.insertRobot("2", "robot2", "large", "typeA", "baseLocationB", "currentLocationB", "inactive", "100", {4, 5, 6}, {4, 5, 6}, 100, 0, 0, 0);
        robotAdapter.insertRobot("3", "robot3", "small", "typeB", "baseLocationC", "currentLocationC", "active", "100", {7, 8}, {7, 8}, 100, 0, 0, 0);

        auto allRobots = robotAdapter.getAllRobots();
        REQUIRE(allRobots.size() >= 2);  // At least two robots should exist

        bool foundRobot2 = false;
        bool foundRobot3 = false;

        for (const auto& robotDoc : allRobots) {
            std::string robotJson = bsoncxx::to_json(robotDoc);
            if (robotJson.find("robot2") != std::string::npos) {
                foundRobot2 = true;
            }
            if (robotJson.find("robot3") != std::string::npos) {
                foundRobot3 = true;
            }
        }
        REQUIRE(foundRobot2);
        REQUIRE(foundRobot3);
        robotAdapter.deleteRobot("2");
        robotAdapter.deleteRobot("3");
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

   //testing updating floor neighbors 
    SECTION("Update Floor Neighbors") {
        floorAdapter.updateNeighbors("1", {2,3});
        auto foundFloor = floorAdapter.findDocumentById("1");
        REQUIRE(foundFloor);

        auto neighborsArray = foundFloor->view()["neighbors"].get_array().value;
        std::vector<int> neighborsVec;
        for (const auto& neighbor : neighborsArray) {
            neighborsVec.push_back(neighbor.get_int32());
        }

        REQUIRE(std::find(neighborsVec.begin(), neighborsVec.end(), 2) != neighborsVec.end());
        REQUIRE(std::find(neighborsVec.begin(), neighborsVec.end(), 3) != neighborsVec.end());
    }

    //testing delete floor
    SECTION("Delete Floor") {
        floorAdapter.deleteFloor("1");
        auto foundFloor = floorAdapter.findDocumentById("1");
        REQUIRE(!foundFloor);
    }
}

/**
 * unit tests for error adapter
 */
TEST_CASE("Error Adapter Unit Tests") {
    auto db = client["test_database"];
    auto collection = db["errors"];
    ErrorAdapter errorAdapter(collection);

    // Testing inserting and finding error
    SECTION("Insert and Find Error") {
        errorAdapter.insertError("1", "1", "Out of Battery", 0);
        auto foundError = errorAdapter.findDocumentById("1");
        REQUIRE(foundError);
        REQUIRE(bsoncxx::to_json(*foundError).find("Out of Battery") != std::string::npos);

        // Check if an exception is throwed when inserting an error with a duplicate ID
        REQUIRE_THROWS(errorAdapter.insertError("1", "2", "Random Break", 0));
    }

    // Testing finding errors by robot ID
    SECTION("Find Error by Robot ID") {
        errorAdapter.insertError("2", "2", "Random Break", 1);
        errorAdapter.insertError("3", "2", "Out of Battery", 1);

        auto errorsForRobot2 = errorAdapter.findErrorByRobotID("2");
        REQUIRE(errorsForRobot2.size() == 2);  
        REQUIRE(bsoncxx::to_json(errorsForRobot2[0]).find("Random Break") != std::string::npos);
        REQUIRE(bsoncxx::to_json(errorsForRobot2[1]).find("Out of Battery") != std::string::npos);
        errorAdapter.deleteError("3");
    }

    // Testing updating error
    SECTION("Update Error") {
        errorAdapter.updateError("1", "1", "Out of Battery", 1);  // Resolve the error
        auto updatedError = errorAdapter.findDocumentById("1");
        REQUIRE(updatedError);
        auto view = updatedError->view();
        REQUIRE(view["resolved"].get_int32() == 1);
        errorAdapter.deleteError("1");
    }

    // Testing deleting an error
    SECTION("Delete Error") {
        errorAdapter.deleteError("2");
        auto foundError = errorAdapter.findDocumentById("2");
        REQUIRE(!foundError);
    }

    // Testing getting all errors
    SECTION("Get All Errors") {
        errorAdapter.insertError("4", "3", "Out of Battery", 0);
        errorAdapter.insertError("5", "4", "Random Break", 1);

        auto allErrors = errorAdapter.getAllErrors();
        REQUIRE(allErrors.size() >= 2);  // At least two errors should exist in the collection
        bool foundOutOfBattery = false;
        bool foundRandomBreak = false;

        for (const auto& errorDoc : allErrors) {
            std::string errorJson = bsoncxx::to_json(errorDoc);
            if (errorJson.find("Out of Battery") != std::string::npos) {
                foundOutOfBattery = true;
            }
            if (errorJson.find("Random Break") != std::string::npos) {
                foundRandomBreak = true;
            }
        }
        REQUIRE(foundOutOfBattery);
        REQUIRE(foundRandomBreak);
        errorAdapter.deleteError("4");
        errorAdapter.deleteError("5");
    }

   //testing if all errors are resolved
    SECTION("All Errors Are Resolved") {
        errorAdapter.insertError("6", "5", "Out of Battery", 1);
        errorAdapter.insertError("7", "5", "Random Break", 1);
        
        //check all errors are resolved for the robot with ID "5"
        REQUIRE(errorAdapter.allErrorIsResolved("5"));

        //Add an unresolved error for the same robot
        errorAdapter.insertError("8", "5", "Out of Battery", 0);
        
        //check that not all errors are resolved for the robot with ID "5"
        REQUIRE(!errorAdapter.allErrorIsResolved("5"));
        errorAdapter.deleteError("7");
        errorAdapter.deleteError("8");
    }

    //testing resolveError
    SECTION("Resolve Error") {
        //insert unresolved error for a robot
        errorAdapter.insertError("9", "6", "Out of Battery", 0);
        
        auto unresolvedError = errorAdapter.findDocumentById("9");
        REQUIRE(unresolvedError);
        REQUIRE(unresolvedError->view()["resolved"].get_int32() == 0);

        errorAdapter.resolveError("6");
        
        auto resolvedError = errorAdapter.findDocumentById("9");
        REQUIRE(resolvedError);
        REQUIRE(resolvedError->view()["resolved"].get_int32() == 1);
        errorAdapter.deleteError("6");
        errorAdapter.deleteError("9");
    }
    
}
