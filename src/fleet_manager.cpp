#include "sys_manager/fleet_manager.hpp"

#include <fstream>
#include <sstream>
#include <vector>
#include <string>

void Fleet_manager::retrive_status(std::string filepath, std::string robot_name, std::string room_name) {
    ofstream myfile(filepath);
    if (myfile.is_open()) {
        myfile << "Robot: " + robot_name + " has cleaned room: " + room_name;
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
            info_lists[0].push_back(line);
        } 
        else if (word == "Floor") {
            info_lists[1].push_back(line);
        } 
        else if (word == "Tasks") {
            info_lists[2].push_back(line);
        }
    }
    
    file.close();
    return info_lists;
}
