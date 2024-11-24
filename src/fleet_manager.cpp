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
    
    // Subscribe to these two events upon initialization
    subscribe(Event::FiveSecReport);
    subscribe(Event::FinalReport);
    subscribe(Event::UpdateFloorNeighbors);
    subscribe(Event::UpdateRobotParameters);

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

    // populate the simulation from the database
    // populate robots
    try {
        auto robots = robot_adapter_.getAllRobots();
        for (const auto& robot : robots) {
            int id = std::stoi(robot.view()["_id"].get_utf8().value.to_string());
            std::string name = robot.view()["name"].get_utf8().value.to_string();
            std::string size = robot.view()["size"].get_utf8().value.to_string();
            std::string type = robot.view()["type"].get_utf8().value.to_string();
            std::string charging_position = robot.view()["base_location"].get_utf8().value.to_string();
            std::string current_position = robot.view()["current_location"].get_utf8().value.to_string();
            std::string status = robot.view()["status"].get_utf8().value.to_string();
            std::string remaining_capacity = robot.view()["capacity"].get_utf8().value.to_string();
            std::vector<int> task_queue;
            // Extract task_queue as a BSON array
            auto taskQueueArray = robot.view()["task_queue"].get_array().value;
            // Convert BSON array to std::vector<int>
            for (const auto& element : taskQueueArray) {
                task_queue.push_back(element.get_int32());
            }
            std::vector<int> path;
            // Extract task_queue as a BSON array
            auto pathArray = robot.view()["path"].get_array().value;
            // Convert BSON array to std::vector<int>
            for (const auto& step : pathArray) {
                path.push_back(step.get_int32());
            }
            // int current_battery = std::stoi(robot.view()["current_battery"].get_utf8().value.to_string());
            // int total_battery_used = std::stoi(robot.view()["total_battery_used"].get_utf8().value.to_string());
            // int error_count = std::stoi(robot.view()["error_count"].get_utf8().value.to_string());
            // int rooms_cleaned = std::stoi(robot.view()["rooms_cleaned"].get_utf8().value.to_string());
            int current_battery = 0;
            int total_battery_used = 0;
            int error_count = 0;
            int rooms_cleaned = 0;
            if (robot.view()["current_battery"].type() == bsoncxx::type::k_int32) {
                current_battery = robot.view()["current_battery"].get_int32();
            } else if (robot.view()["current_battery"].type() == bsoncxx::type::k_int64) {
                current_battery = static_cast<int>(robot.view()["current_battery"].get_int64());
            } else {
                throw std::runtime_error("Unexpected type for current_battery field");
            }
            if (robot.view()["total_battery_used"].type() == bsoncxx::type::k_int32) {
                total_battery_used = robot.view()["total_battery_used"].get_int32();
            } else if (robot.view()["total_battery_used"].type() == bsoncxx::type::k_int64) {
                total_battery_used = static_cast<int>(robot.view()["total_battery_used"].get_int64());
            } else {
                throw std::runtime_error("Unexpected type for total_battery_used field");
            }
            if (robot.view()["rooms_cleaned"].type() == bsoncxx::type::k_int32) {
                rooms_cleaned = robot.view()["rooms_cleaned"].get_int32();
            } else if (robot.view()["rooms_cleaned"].type() == bsoncxx::type::k_int64) {
                rooms_cleaned = static_cast<int>(robot.view()["rooms_cleaned"].get_int64());
            } else {
                throw std::runtime_error("Unexpected type for rooms_cleaned field");
            }

            // Retrieve and process "error_count"
            if (robot.view()["error_count"].type() == bsoncxx::type::k_int32) {
                error_count = robot.view()["error_count"].get_int32();
            } else if (robot.view()["error_count"].type() == bsoncxx::type::k_int64) {
                error_count = static_cast<int>(robot.view()["error_count"].get_int64());
            } else {
                throw std::runtime_error("Unexpected type for error_count field");
            }

            RobotSize rsSize = to_enum_robot_size(size);
            RobotType rtType = to_enum_robot_type(type);
            RobotStatus rsStatus = to_enum_robot_status(status);
            simulator_.add_robot(id, name, rsSize, rtType, std::stoi(charging_position), std::stoi(current_position), rsStatus, std::stoi(remaining_capacity), task_queue, path, current_battery, total_battery_used, error_count, rooms_cleaned);

            if (id > robot_id_count) {
                robot_id_count = id;
            }
        }
    } catch (const std::exception& e){
        std::cerr << "Error populating simulation from DB robot collection: " << e.what() << endl;
    }

    // populate floors
    try {
        auto floors = floor_adapter_.getAllFloors();
        for (const auto& floor : floors) {
            int id = std::stoi(floor.view()["_id"].get_utf8().value.to_string());
            std::string name = floor.view()["name"].get_utf8().value.to_string();
            std::string room_type_str = floor.view()["room_type"].get_utf8().value.to_string();
            std::string floor_type_str = floor.view()["floor_type"].get_utf8().value.to_string();
            std::string size = floor.view()["size"].get_utf8().value.to_string();
            std::string interaction_level_str = floor.view()["interaction_level"].get_utf8().value.to_string();
            std::string restricted_str = floor.view()["restricted"].get_utf8().value.to_string();
            std::string clean_level = floor.view()["clean_level"].get_utf8().value.to_string();

            std::vector<int> neighbors;
            auto neighbors_array = floor.view()["neighbors"].get_array().value;
            for (const auto& neighbor : neighbors_array) {
                neighbors.push_back(neighbor.get_int32().value);
            }

            FloorRoomType room_type = to_enum_floor_room_type(room_type_str);
            FloorType floor_type = to_enum_floor_type(floor_type_str);
            FloorSize floor_size = to_enum_floor_size(size);
            FloorInteraction interaction_level = to_enum_floor_interaction(interaction_level_str);
            bool restricted = restricted_str == "true";
            
            simulator_.add_floor(id, name, room_type, floor_type, floor_size, interaction_level, restricted, std::stoi(clean_level), neighbors);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error populating floors: " << e.what() << std::endl;
    }

    
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
    if (event == Event::FiveSecReport) {
        handle_five_sec_ping(data);
    } else if (event == Event::FinalReport) {
        handle_finished_ping(data);
    } else if (event == Event::ErrorReport) {
        std::cout << "Error Report Event" << std::endl;
    } else if (event == Event::DisplayText) {
        std::cout << "DisplayText Event" << std::endl;
    }
}

void FleetManager::update(const Event& event, const int id, const std::vector<int>& data) {
    if (event == Event::UpdateFloorNeighbors) {
        update_neighbors_db(id, data);
    }
}

void FleetManager::update(const types::Event& event, const std::string& id, const std::string& currentLocation, const std::string& status, const std::string& capacity, 
                    const std::vector<int>& taskQueue, const std::vector<int>& path, const int& currentBattery, const int& totalBatteryUsed) {
    if (event == Event::UpdateRobotParameters) {
        robot_adapter_.updateRobot(id, currentLocation, status, capacity, taskQueue, path, currentBattery, totalBatteryUsed);
    }
}

// Notify all the subscribers
void FleetManager::notify(const Event& event, const std::string& data) {
    for (auto& subscriber : subscribers_[event]) {
        subscriber->update(event, data);
    }
}

void FleetManager::notify(const Event& event, const int id, const std::vector<int>& data) {
    for (auto& subscriber : subscribers_[event]) {
        subscriber->update(event, id, data);
    }
}

void FleetManager::notify(const types::Event& event, const std::string& id, const std::string& currentLocation, const std::string& status, const std::string& capacity, 
                    const std::vector<int>& taskQueue, const std::vector<int>& path, const int& currentBattery, const int& totalBatteryUsed) {
    for (auto& subscriber : subscribers_[event]) {
        subscriber->update(event, id, currentLocation, status, capacity, taskQueue, path, currentBattery, totalBatteryUsed);
    }
}

void FleetManager::subscribe(Subscriber* subscriber, const Event& event) {
    subscribers_[event].push_back(subscriber);
}

// Let the subscriber unsubscribe from an event
void FleetManager::unsubscribe(Subscriber* subscriber, const Event& event) {
    auto& subs = subscribers_[event];
    subs.erase(std::remove(subs.begin(), subs.end(), subscriber), subs.end());
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

void FleetManager::update_neighbors_db(const int id, const std::vector<int>& data) {
    floor_adapter_.updateNeighbors(std::to_string(id), data);
}

void FleetManager::update_robot_db(const std::string& id, const std::string& currentLocation, const std::string& status, const std::string& capacity, 
                    const std::vector<int>& taskQueue, const std::vector<int>& path, const int& currentBattery, const int& totalBatteryUsed) {
    robot_adapter_.updateRobot(id, currentLocation, status, capacity, taskQueue, path, currentBattery, totalBatteryUsed);
}


// Wrapper method that just calls the add_robot for the sim and the db
int FleetManager::add_robot(std::string name, std::string size, std::string type, std::string charging_position, std::string current_position, int capacity, std::vector<int> task_queue, std::vector<int> path, int total_battery_used, int error_count, int rooms_cleaned) {
    RobotSize RsSize = to_enum_robot_size(size);
    RobotType RtType = to_enum_robot_type(type);

    if (robot_id_count >= 11) {
        std::cerr << "Error: reached the limit of robots" << std::endl;
        return false;
    }
    robot_id_count += 1;
    try {
        robot_adapter_.insertRobot(std::to_string(robot_id_count), name, size, type, charging_position, current_position, types::to_string(RobotStatus::Available), "100", {}, {}, 100, 0, 0, 0);
        simulator_.add_robot(robot_id_count, name, RsSize, RtType, std::stoi(charging_position), std::stoi(current_position), RobotStatus::Available, 100, {}, {}, 100, 0, 0, 0);
    } catch (const std::exception& e) {
        std::cerr << "Error adding robot to the database: " << e.what() << std::endl;
        return false;
    }

    return true;
}

// Wrapper method that just calls the add_floor for the sim and the db
int FleetManager::add_floor(std::string name, std::string roomType, std::string type, std::string size, std::string interaction, std::vector<int> neighbors) {
    FloorRoomType FrtRoom = to_enum_floor_room_type(roomType);
    FloorType FtType = to_enum_floor_type(type);
    FloorSize FsSize = to_enum_floor_size(size);
    FloorInteraction FiInteraction = to_enum_floor_interaction(interaction);


    if (floor_id_count >= 11) {
        std::cerr << "Error: reached the limit of floors" << std::endl;
        return false;
    }

    floor_id_count += 1;
    try {
        floor_adapter_.insertFloor(std::to_string(floor_id_count), name, roomType, type, size, interaction, "false", "50", neighbors);
        simulator_.add_floor(floor_id_count, name, FrtRoom, FtType, FsSize, FiInteraction, false, 50, neighbors);
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

bool FleetManager::add_task_to_back(int robot_id, std::vector<int> floor_ids) {
    try {
        simulator_.add_task_to_back(robot_id, floor_ids);      
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error adding tasks to back: " << e.what() << std::endl;
        return false;
    }

}