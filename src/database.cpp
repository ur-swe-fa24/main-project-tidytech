#include "database/database.hpp"

// Convert available int to string
string available_to_string(int available) {
    return available == 1 ? "Available" : "Not Available";
}

// Create robot
void Database::add_robot(int id, const string& type, int available, int location) {
    robots_[id] = {type, available_to_string(available), to_string(location)};
}

// Create floor
void Database::add_floor(int id, const string& name, const string& type) {
    floors_[id] = {name, type};
}

// Create task
void Database::add_task(int id, const vector<int>& robot_assigned, const vector<int>& room_assigned, const string& status) {
    string robot_ids = "";
    for (const auto& bot_id : robot_assigned) {
        robot_ids += to_string(bot_id) + " ";
    }
    
    string room_ids = "";
    for (const auto& roomid : room_assigned) {
        room_ids += to_string(roomid) + " ";
    }

    tasks_[id] = {robot_ids, room_ids, status};
}

// Read robot
vector<string> Database::get_robot(int id) {
    if (robots_.find(id) != robots_.end()) {
        return robots_[id];
    }
    return {};
}

// Read floor
vector<string> Database::get_floor(int id) {
    if (floors_.find(id) != floors_.end()) {
        return floors_[id];
    }
    return {};
}

// Read task
vector<string> Database::get_task(int id) {
    if (tasks_.find(id) != tasks_.end()) {
        return tasks_[id];
    }
    return {};
}

// Update robot
void Database::update_robot(int id, const string& type, int available, int location) {
    if (robots_.find(id) != robots_.end()) {
        robots_[id] = {type, available_to_string(available), to_string(location)};
    }
}

// Update floor
void Database::update_floor(int id, const string& name, const string& type) {
    if (floors_.find(id) != floors_.end()) {
        floors_[id] = {name, type};
    }
}

// Update task
void Database::update_task(int id, const vector<int>& robot_assigned, const vector<int>& room_assigned, const string& status) {
    if (tasks_.find(id) != tasks_.end()) {
        string robot_ids = "";
        for (const auto& bot_id : robot_assigned) {
            robot_ids += to_string(bot_id) + " ";
        }
        
        string room_ids = "";
        for (const auto& room_id : room_assigned) {
            room_ids += to_string(room_id) + " ";
        }

        tasks_[id] = {robot_ids, room_ids, status};
    }
}

// Delete robot
void Database::delete_robot(int id) {
    robots_.erase(id);
}

// Delete floor
void Database::delete_floor(int id) {
    floors_.erase(id);
}

// Delete task
void Database::delete_task(int id) {
    tasks_.erase(id);
}
