#include "sys_manager/fleet_manager.hpp"
#include "ui/user_interface.hpp"
#include "types/types.hpp"

#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace types;

FleetManager::FleetManager() : simulator_{}, dbmanager_{DBManager::getInstance("mongodb://localhost:27017", "database")}, 
                                robot_adapter_{dbmanager_.getDatabase()["robots"]}, floor_adapter_{dbmanager_.getDatabase()["floors"]} {
    // get the last robot id
    auto last_robot = dbmanager_.getDatabase()["robots"].find_one(
        bsoncxx::builder::basic::make_document(),
        mongocxx::options::find{}.sort(
            bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("_id", -1))
        )
    );
    if (last_robot) {
        robot_id_count = std::stoi(last_robot->view()["_id"].get_utf8().value.to_string());
    } else {
        robot_id_count = 0;
    }
    std::cout << "robot count is: " << robot_id_count << endl;
    // get the last floor id
    auto last_floor = dbmanager_.getDatabase()["floors"].find_one(
    bsoncxx::builder::basic::make_document(),
    mongocxx::options::find{}.sort(
        bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("_id", -1))
    )
    );
    if (last_floor) {
        floor_id_count = std::stoi(last_floor->view()["_id"].get_utf8().value.to_string());
    } else {
        floor_id_count = 0;
    } 
    std::cout << "floor count is: " << floor_id_count << endl;

    // populate the simulation by using DB data
    try {
        auto robots = robot_adapter_.getAllRobots();
        for (const auto& robot : robots) {
            int id = std::stoi(robot.view()["_id"].get_utf8().value.to_string());
            std::string name = robot.view()["name"].get_utf8().value.to_string();
            std::string size = robot.view()["size"].get_utf8().value.to_string();
            std::string type = robot.view()["type"].get_utf8().value.to_string();
            std::string charging_position = robot.view()["baseLocation"].get_utf8().value.to_string();
            std::string current_position = robot.view()["currentLocation"].get_utf8().value.to_string();
            std::string status = robot.view()["status"].get_utf8().value.to_string();

            RobotSize rsSize = to_enum_robot_size(size);
            RobotType rtType = to_enum_robot_type(type);
            RobotStatus rsStatus = to_enum_robot_status(status);
            simulator_.add_robot(id, name, rsSize, rtType, std::stoi(charging_position), std::stoi(current_position), rsStatus);

            if (id > robot_id_count) {
                robot_id_count = id;
            }
        }
    } catch (const std::exception& e){
        std::cerr << "Error populating simulation from DB robot collection: " << e.what() << endl;
    }

    // Subscribe to these two events upon initialization
    subscribe(Event::FiveSecReport);
    subscribe(Event::FinalReport);
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

void FleetManager::subscribe(const Event& event) {
    // subscribe to an event
    simulator_.subscribe(this, event);
}

void FleetManager::unsubscribe(const Event& event) {
    // unsubscribe from an event
    simulator_.unsubscribe(this, event);
}

void FleetManager::update(const Event& event, const std::string& data) {
    // Do a particular method depending on what type of event is being updated
    switch (event) {
        case Event::FiveSecReport:
            handle_five_sec_ping(data);
            break;
        case Event::FinalReport:
            handle_finished_ping(data);
            break;
        case Event::ErrorReport:
            // TODO
            break;
        case Event::DisplayText:
            break;
    }
}

void FleetManager::handle_five_sec_ping(const std::string& data) {
    // Prints data
    std::cout << data << std::endl;
}

void FleetManager::handle_finished_ping(const std::string& data) {
    // Calls write_output to write data to a file
    std::string message = "Final Report Summary:\n" + data;
    notify(Event::DisplayText, data);
    // write_output("../app/output.txt", message);
}


void FleetManager::subscribe(Subscriber* subscriber, const Event& event) {
    subscribers_[event].push_back(subscriber);
}

// Let the subscriber unsubscribe from an event
void FleetManager::unsubscribe(Subscriber* subscriber, const Event& event) {
    auto& subs = subscribers_[event];
    subs.erase(std::remove(subs.begin(), subs.end(), subscriber), subs.end());
}

// Notify all the subscribers
void FleetManager::notify(const Event& event, const std::string& data) {
    for (auto& subscriber : subscribers_[event]) {
        subscriber->update(event, data);
    }
}

// Wrapper method that just calls the add_robot for the sim and the db
int FleetManager::add_robot(std::string name, std::string size, std::string type, std::string charging_position, std::string current_position, std::string capacity) {
    RobotSize RsSize;
    if (size == "Small") {
        RsSize = RobotSize::Small;
    } else if (size == "Medium") {
        RsSize = RobotSize::Medium;
    } else if (size == "Large") {
        RsSize = RobotSize::Large;
    } else {
        std::cout << "Invalid Robot Size" << std::endl;
        return false;
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
    if (robot_id_count >= 11) {
        std::cerr << "Error: reached the limit of robots" << std::endl;
        return false;
    }
    robot_id_count += 1;
    try {
        robot_adapter_.insertRobot(std::to_string(robot_id_count), name, size, type, charging_position, current_position, types::to_string(RobotStatus::Available), "100");
        simulator_.add_robot(robot_id_count, name, RsSize, RtType, std::stoi(charging_position), std::stoi(current_position), RobotStatus::Available);
    } catch (const std::exception& e) {
        std::cerr << "Error adding robot to the database: " << e.what() << std::endl;
        return false;
    }

    return true;
}

// Wrapper method that just calls the add_floor for the sim and the db
int FleetManager::add_floor(std::string name, std::string roomType, std::string type, std::string size, std::string interaction, std::vector<int> neighbors) {
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
        return false;
    }

    if (type == "Carpet") {
        FtType = FloorType::Carpet;
    } else if (type == "Wood") {
        FtType = FloorType::Wood;
    } else if (type == "Tile") {
        FtType = FloorType::Tile;
    } else {
        std::cout << "Invalid Floor Type" << std::endl;
        return false;
    }

    if (size == "Small") {
        FsSize = FloorSize::Small;
    } else if (size == "Medium") {
        FsSize = FloorSize::Medium;
    } else if (size == "Large") {
        FsSize = FloorSize::Large;
    } else {
        std::cout << "Invalid Floor Size" << std::endl;
        return false;
    }

    if (interaction == "Low") {
        FiInteraction = FloorInteraction::Low;
    } else if (interaction == "Moderate") {
        FiInteraction = FloorInteraction::Moderate;
    } else if (interaction == "High") {
        FiInteraction = FloorInteraction::High;  
    } else {
        std::cout << "Invalid Floor Interaction" << std::endl;
        return false;
    }

    if (floor_id_count >= 11) {
        std::cerr << "Error: reached the limit of floors" << std::endl;
        return false;
    }
    floor_id_count += 1;
    try {
        floor_adapter_.insertFloor(std::to_string(floor_id_count), name, roomType, type, size, interaction, "Not Restricted", "100");
        simulator_.add_floor(floor_id_count, name, FrtRoom, FtType, FsSize, FiInteraction, false, 100, neighbors);
    } catch (const std::exception& e) {
        std::cerr << "Error adding floor to the database: " << e.what() << std::endl;
        return false;
    }
    return true;
}

// Wrapper that just calls the get_all_floor_names() from the sim
std::vector<std::string> FleetManager::get_all_floor_names() {
    return simulator_.get_all_floor_names();
}

std::vector<std::string> FleetManager::get_all_robot_names() {
    return simulator_.get_all_robot_names();
}

bool FleetManager::add_task(int robot_id, int floor_id) {
    simulator_.add_task(robot_id, floor_id);
    return true;
}