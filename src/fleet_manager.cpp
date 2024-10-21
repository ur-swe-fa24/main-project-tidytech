#include "sys_manager/fleet_manager.hpp"

#include <fstream>
#include <sstream>
#include <vector>
#include <string>

Fleet_manager::Fleet_manager(Database db) {
    database_ = db;
};

void Fleet_manager::write_output(std::string filepath, std::string message) {
    ofstream myfile(filepath);
    if (myfile.is_open()) {
        myfile << message + "\n";
        myfile.close();
    } else {
        throw std::invalid_argument("invalid file path.");
    }
}

vector<vector<string>> Fleet_manager::read_ui_input(std::string filepath) {
    ifstream file(filepath);
    string line;
    vector<vector<string>> info_lists(3);
    
    if (!file.is_open()) {
        throw std::invalid_argument("invalid file path.");
        return info_lists;
    }

    while (getline(file, line)) {
        stringstream ss(line);
        string word;
        ss >> word;

        if (word == "Robot") {
            int id, available, location;
            string type;
            ss >> id >> type >> available >> location;
            this->database_.add_robot(id, type, available, location);
            this->simulator_.add_robot(std::to_string(id), "small", type, std::to_string(location), std::to_string(location));
        } else if (word == "Floor") {
            int id;
            string name, type;
            ss >> id >> name >> type;
            this->database_.add_floor(id, name, type);
        } else if (word == "Tasks") {
            vector<int> robot_assigned_vector;
            vector<int> room_assigned_vector;
            int id;
            int robot_assigned;
            int room_assigned;
            string status;
            ss >> id >> robot_assigned >> room_assigned >> status;
            robot_assigned_vector.push_back(robot_assigned);
            room_assigned_vector.push_back(room_assigned);
            this->database_.add_task(id, robot_assigned_vector, room_assigned_vector, status);
            this->simulator_.clean(std::to_string(robot_assigned), std::to_string(room_assigned));
        }
        // Change for future
        simulator_.start_simulation();
    }
    
    file.close();
    return info_lists;
}
