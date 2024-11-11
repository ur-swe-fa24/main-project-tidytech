#include "sys_manager/fleet_manager.hpp"
#include "ui/user_interface.hpp"
#include "types/types.hpp"

#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace types;

int FleetManager::robot_count = 0; // For robot_id
int FleetManager::floor_count = 0; // For floor_id


// TODO: change it in sprint 4
FleetManager::FleetManager() : simulator_{}, dbmanager_{DBManager::getInstance("mongodb://localhost:27017", "database")}, 
                                robot_adapter_{dbmanager_.getDatabase()["robots"]}, floor_adapter_{dbmanager_.getDatabase()["floors"]} {
    // Subscribe to these two events upon initialization
    subscribe("five_sec_ping");
    subscribe("finished_ping");
}

void FleetManager::write_output(string filepath, string message) {
    // Writes the message parameter to the file specified by filepath
    ofstream myfile(filepath);
    if (myfile.is_open()) {
        myfile << message + "\n";
        myfile.close();
    } else {
        throw invalid_argument("invalid file path.");
    }
}

void FleetManager::subscribe(const std::string& event) {
    // subscribe to an event
    simulator_.subscribe(this, event);
}

void FleetManager::unsubscribe(const std::string& event) {
    // unsubscribe from an event
    simulator_.unsubscribe(this, event);
}

void FleetManager::update(const std::string& event, const std::string& data) {
    // Do a particular method depending on what type of event is being updated
    if (event == "five_sec_ping") {
        handle_five_sec_ping(data);
    } else if (event == "finished_ping") {
        handle_finished_ping(data);
    }
}

void FleetManager::handle_five_sec_ping(const std::string& data) {
    // Prints data
    std::cout << data << std::endl;
}

void FleetManager::handle_finished_ping(const std::string& data) {
    // Calls write_output to write data to a file
    std::string message = "Final Report Summary:\n" + data;
    notify("display_text", data);
    // write_output("../app/output.txt", message);
}


void FleetManager::subscribe(Subscriber* subscriber, const std::string& event) {
    subscribers_[event].push_back(subscriber);
}

// Let the subscriber unsubscribe from an event
void FleetManager::unsubscribe(Subscriber* subscriber, const std::string& event) {
    auto& subs = subscribers_[event];
    subs.erase(std::remove(subs.begin(), subs.end(), subscriber), subs.end());
}

// Notify all the subscribers
void FleetManager::notify(const std::string& event, const std::string& data) {
    for (auto& subscriber : subscribers_[event]) {
        subscriber->update(event, data);
    }
}

// Wrapper method that just calls the add_robot for the sim and the db
void FleetManager::add_robot(std::string name, std::string size, std::string type, std::string charging_position, std::string current_position) {
    RobotSize RsSize;
    if (size == "Small") {
        RsSize = RobotSize::Small;
    } else if (size == "Medium") {
        RsSize = RobotSize::Medium;
    } else if (size == "Large") {
        RsSize = RobotSize::Large;
    } else {
        std::cout << "Invalid Robot Size" << std::endl;
    }

    RobotType RtType;
    if (type == "Shampoo") {
        RtType = RobotType::Shampoo;
    } else if (type == "Vacuum") {
        RtType = RobotType::Vaccum;
    } else if (type == "Scrubber") {
        RtType = RobotType::Scrubber;
    } else {
        std::cout << "Invalid Robot Type" << std::endl;
    }
    
    try {
        simulator_.add_robot(++robot_count, name, RsSize, RtType, charging_position, current_position, RobotStatus::Available);
        robot_adapter_.insertRobot(std::to_string(robot_count), name, size, type, charging_position, current_position, types::to_string(RobotStatus::Available));
    } catch (const std::exception& e) {
        std::cerr << "Error adding robot to the database: " << e.what() << std::endl;
    }
    // database_.add_robot(id, type, 1, location);
}

// Wrapper method that just calls the add_floor for the sim and the db
void FleetManager::add_floor(std::string name, std::string roomType, std::string type, std::string size, std::string interaction, std::vector<int> neighbors) {
    // add_floor(int id, FloorRoomType room, FloorType floortype, FloorSize size, FloorInteraction interaction_level, bool restriction, int clean_level, std::vector<int> neighbors)
    FloorSize FsSize;
    FloorType FtType;
    FloorRoomType FrtRoom;
    FloorInteraction FiInteraction;
    if (roomType == "Elevator") {
        FrtRoom = FloorRoomType::Elevator;
    } else if (roomType == "Hallway") {
        FrtRoom = FloorRoomType::Hallway;
    } else if (roomType == "Room") {
        FrtRoom = FloorRoomType::Room;
    } else {
        std::cout << "Invalid Floor Room Type" << std::endl;
    }

    if (type == "Carpet") {
        FtType = FloorType::Carpet;
    } else if (type == "Wood") {
        FtType = FloorType::Wood;
    } else if (type == "Tile") {
        FtType = FloorType::Tile;
    } else {
        std::cout << "Invalid Floor Type" << std::endl;
    }

    if (size == "Small") {
        FsSize = FloorSize::Small;
    } else if (size == "Medium") {
        FsSize = FloorSize::Medium;
    } else if (size == "Large") {
        FsSize = FloorSize::Large;
    } else {
        std::cout << "Invalid Floor Size" << std::endl;
    }

    if (interaction == "Low") {
        FiInteraction = FloorInteraction::Low;
    } else if (interaction == "Moderate") {
        FiInteraction = FloorInteraction::Moderate;
    } else if (interaction == "High") {
        FiInteraction = FloorInteraction::High;  
    } else {
        std::cout << "Invalid Floor Interaction" << std::endl;
    }

    
    simulator_.add_floor(++floor_count, name, FrtRoom, FtType, FsSize, FiInteraction, false, 100, neighbors);
    floor_adapter_.insertFloor(std::to_string(floor_count), name, roomType, type, size, interaction, "Not Restricted", "100");
    // database_.add_robot(id, type, 1, location);
}

// Wrapper that just calls the get_all_floor_names() from the sim
std::vector<std::string> FleetManager::get_all_floor_names() {
    return simulator_.get_all_floor_names();
}