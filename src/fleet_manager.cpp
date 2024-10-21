#include "sys_manager/fleet_manager.hpp"

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

Fleet_manager::Fleet_manager(Database db) {
    database_ = db;
};

void Fleet_manager::write_output(string filepath, string message) {
    ofstream myfile(filepath);
    if (myfile.is_open()) {
        myfile << message + "\n";
        myfile.close();
    } else {
        throw invalid_argument("invalid file path.");
    }
}

void Fleet_manager::read_ui_input(string filepath) {
    ifstream file(filepath);
    string line;
    vector<vector<string>> info_lists(3);
    
    if (!file.is_open()) {
        throw invalid_argument("invalid file path.");
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
            this->notify({to_string(id), "small", type, to_string(location), to_string(location)});
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
        }
        // Change for future
    }
    
    file.close();
}

void Fleet_manager::update(vector<string> inputs) {
    Fleet_manager::write_output("../app/status_report.txt", inputs[0]);
}


void Fleet_manager::notify(vector<string> outputs) {
    for (auto& sub : subscribers_) {
        sub.update(outputs);
    }
}

void Fleet_manager::add_subs(Subscriber& sub) {
    subscribers_.push_back(sub);
}
