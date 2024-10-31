#include "sys_manager/fleet_manager.hpp"

#include <fstream>
#include <sstream>
#include <vector>
#include <string>

FleetManager::FleetManager(Database &db) : simulator_{}, database_(db){
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

void FleetManager::read_ui_input(string filepath) {
    // Reads file from the filepath
    ifstream file(filepath);
    string line;
    
    if (!file.is_open()) {
        throw invalid_argument("invalid file path.");
    }

    // Adds to the database and simulation depending on what type of "thing" it is
    // TODO: abstract this and thread this
    while (getline(file, line)) {
        stringstream ss(line);
        string word;
        ss >> word;

        if (word == "Robot") {
            int id, available, location;
            string type;
            ss >> id >> type >> available >> location;
            database_.add_robot(id, type, available, location);
            simulator_.add_robot(to_string(id), "small", type, to_string(location), to_string(location));
        } else if (word == "Floor") {
            int id;
            string name, type;
            ss >> id >> name >> type;
            database_.add_floor(id, name, type);
        } else if (word == "Tasks") {
            vector<int> robot_assigned_vector; // TODO: change this
            vector<int> room_assigned_vector; // TODO: change this
            int id;
            int robot_assigned;
            int room_assigned;
            string status;
            ss >> id >> robot_assigned >> room_assigned >> status;
            robot_assigned_vector.push_back(robot_assigned);
            room_assigned_vector.push_back(room_assigned);
            database_.add_task(id, robot_assigned_vector, room_assigned_vector, status);
            simulator_.add_task(to_string(robot_assigned), to_string(room_assigned));
        }
    }
    
    file.close();
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
    write_output("../app/output.txt", message);
}
