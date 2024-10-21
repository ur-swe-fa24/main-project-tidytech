#include "sys_manager/fleet_manager.hpp"

#include <fstream>
#include <sstream>
#include <vector>
#include <string>

Fleet_manager::Fleet_manager(Simulator* simulation, Database* db)
    : simulator_(simulation), database_(db) {
    subscribe("five_sec_ping");
    subscribe("finished_ping");
}

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
            database_->add_robot(id, type, available, location);
        } else if (word == "Floor") {
            int id;
            string name, type;
            ss >> id >> name >> type;
            database_->add_floor(id, name, type);
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
            database_->add_task(id, robot_assigned_vector, room_assigned_vector, status);
        }
    }
    
    file.close();
}

void Fleet_manager::subscribe(const std::string& event) {
    simulator_->subscribe(this, event);
}

void Fleet_manager::unsubscribe(const std::string& event) {
    simulator_->unsubscribe(this, event);
}

void Fleet_manager::update(const std::string& event, const std::string& data) {
    if (event == "five_sec_ping") {
        handle_five_sec_ping(data);
    } else if (event == "finished_ping") {
        handle_finished_ping(data);
    }
}

void Fleet_manager::handle_five_sec_ping(const std::string& data) {
    std::cout << "5 sec ping" + data << std::endl;
}

void Fleet_manager::handle_finished_ping(const std::string& data) {
    std::cout << "Finished ping" + data << std::endl;
}
