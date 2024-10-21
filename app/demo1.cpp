#include <iostream>
#include "database/database.hpp"
#include "simulation/simulator.hpp"
#include "sys_manager/fleet_manager.hpp"

int main() {
    // ========= START DB UNIT TESTING =================
    Database db;

    // Adding robots, included all 3 types (vacuum, scrubber, shampoo)
    db.add_robot(1, "Vacuum", 1, 3); 
    db.add_robot(2, "Shampoo", 0, 2); 
    db.add_robot(3, "Scrubber", 1, 1); 
    
    // Adding floors (id is an int)
    db.add_floor(1, "Main-Floor", "Wood");
    db.add_floor(2, "Lobby", "Carpet");
    db.add_floor(3, "Office1", "Tile");
    db.add_floor(4, "Office2", "Wood");
    
    // Adding tasks with lists of robot and room ids
    vector<int> robot_ids1 = {1,2};
    vector<int> room_ids1 = {2};
    db.add_task(1, robot_ids1, room_ids1, "Waiting to Start"); // Task assigned to robot 1 and 2 in room 2

    vector<int> robot_ids2 = {3}; 
    vector<int> room_ids2 = {1};
    db.add_task(2, robot_ids2, room_ids2, "Completed"); // Task assigned to robot 3 in room 1

    vector<int> robot_ids3 = {3}; 
    vector<int> room_ids3 = {4};
    db.add_task(3, robot_ids3, room_ids3, "In Progress"); // Task assigned to robot 3 in room 4

    // Display robot information
    std::cout << "Robot ? Info: type availability location" << std::endl;
    for (int id = 1; id <= 3; ++id) {
        std::cout << "Robot " << id << " info: ";
        for (const auto& field : db.get_robot(id)) {
            std::cout << field << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // Display floor information
    std::cout << "Floor ? Info: id name type" << std::endl;
    for (int id = 1; id <= 4; ++id) {
        std::cout << "Floor " << id << " info: ";
        for (const auto& field : db.get_floor(id)) {
            std::cout << field << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // Display task information
    std::cout << "Task ? Info: id robot_assigned room_assigned status" << std::endl;
    for (int id = 1; id <= 3; ++id) {
        std::cout << "Task " << id << " info: ";
        for (const auto& field : db.get_task(id)) {
            std::cout << field << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // Update robot
    db.update_robot(1, "Vacuum", 0, 3); // Update to "Not Available"

    // Update task
    db.update_task(1, robot_ids1, room_ids1, "In Progress"); // Update to "In Progress"

    // Display updated robot information
    std::cout << "Updated Robot 1 info: ";
    for (const auto& field : db.get_robot(1)) {
        std::cout << field << " ";
    }
    std::cout << std::endl;

    // Display updated task information
    std::cout << "Updated Task 1 info: ";
    for (const auto& field : db.get_task(1)) {
        std::cout << field << " ";
    }
    std::cout << std::endl;

    // Delete robot and check
    db.delete_robot(2);
    if (db.get_robot(2).empty()) {
        std::cout << "Robot 2 successfully deleted." << std::endl;
    } else {
        std::cout << "Robot 2 still exists." << std::endl;
    }

    // Delete floor and check
    db.delete_floor(2);
    if (db.get_floor(2).empty()) {
        std::cout << "Floor 2 successfully deleted." << std::endl;
    } else {
        std::cout << "Floor 2 still exists." << std::endl;
    }

    // Delete task and check
    db.delete_task(3);
    if (db.get_task(3).empty()) {
        std::cout << "Task 3 successfully deleted." << std::endl;
    } else {
        std::cout << "Task 3 still exists." << std::endl;
    }


    // ========= START INTEGRATION TESTING ==================
    Simulator simulation;
    Database db2;
    Fleet_manager fleet_manager(&simulation, &db2);

    fleet_manager.read_ui_input("../app/input.txt");
    simulation.start_simulation();
}

