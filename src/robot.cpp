#include "simulation/robot.hpp"
#include "spdlog/spdlog.h"

// Robot constructor
// Initialize all variables
Robot::Robot(int id, std::string name, RobotSize size, RobotType type, int base, int curr, RobotStatus status) {
    id_ = id;
    name_ = name;
    size_ = size;
    type_ = type;
    base_ = base;
    curr_ = curr;
    battery_ = 100;
    status_ = status;
    remaining_capacity_ = 100;
    task_queue_ = {};
    curr_path_ = {};
    total_battery_used = 0;
    error_count = 0;
    rooms_cleaned = 0;

} 

// to_string for robot object
std::string Robot::to_string() const {

    std::string str_tasks = "";
    std::string str_path = "";

    for (int task : task_queue_) {
        str_tasks += std::to_string(task) + ", ";
    }

    if (str_tasks.length() >= 2) {
        str_tasks.erase(str_tasks.length() - 2, 2); // Remove last two characters
    }

    std::queue<int> tmp = curr_path_; // Copy curr_path_
    while (!tmp.empty()) { 
        str_path += std::to_string(tmp.front()) + ", ";
        tmp.pop();
    }

    if (str_path.length() >= 2) {
        str_path.erase(str_path.length() - 2, 2); // Remove last two characters
    }

    return "Robot Id: " + std::to_string(id_) +
            ", Name: " + name_ + "\n" +
            "Size: " + types::to_string(size_) +
            ", Type: " + types::to_string(type_) + "\n" +
            "Status: " + types::to_string(status_) +
            ", Battery: " + std::to_string(battery_) + 
            ", Remaining Capacity: " + std::to_string(remaining_capacity_) + "\n" +
            "Base Location: " + std::to_string(base_) + "\n" +
            "Current Location: " + std::to_string(curr_) + "\n" +
            "Current Path: " + str_path + "\n" +
            "Tasks: " + str_tasks + "\n";
            "Total Battery Used: " + std::to_string(total_battery_used) + "\n" +
            "Error Count: " + std::to_string(error_count) + "\n" +
            "Rooms Cleaned: " + std::to_string(rooms_cleaned);
}

// Add tasks at the back of the queue
void Robot::add_tasks_to_back(std::vector<int> floors) {
    task_queue_.insert(task_queue_.end(), floors.begin(), floors.end());
}

// Add tasks at the front of the queue
void Robot::add_tasks_to_front(std::vector<int> floors) {
    task_queue_.insert(task_queue_.begin(), floors.begin(), floors.end());
}

// Move to next task if it can
void Robot::move_to_next_task() {
    if (task_queue_.empty()) {
        spdlog::error("Task queue is empty for robot {}", id_);
        throw std::runtime_error("Cannot move to the next task because the task queue is empty.");
    }
    if (curr_ == task_queue_.front()) {
        // Reached the task floor
        status_ = RobotStatus::Cleaning;
        task_queue_.erase(task_queue_.begin()); //Remove the first task
    } else {
        status_ = RobotStatus::Traveling;
        move_to_next_floor();
    }
    
}

// Charge 5 unit power every call
// Must be at base
void Robot::charge() {
    if (curr_ != base_) {
        spdlog::error("Robot {} cannot charge because it is not at the base. Current location: {}, Base: {}", id_, curr_, base_);
        throw std::runtime_error("Robot must be at the base to charge.");
    }
    battery_ = std::min(100, battery_ + 5);
    if (battery_ == 100) {
        status_ = RobotStatus::Available;
    }
}

// Move to base and change status to Charging
void Robot::go_charge() {
    status_ = RobotStatus::Charging;
    move_to_next_floor();
}

// Move to base and wait to get empty
void Robot::go_empty() {
    status_ = RobotStatus::NeedEmpty;
    move_to_next_floor();
}

// Consuming power at every tick
void Robot::consume_power(int amount) {
    if (amount < 0) {
        spdlog::error("Power consumption amount cannot be negative: {}", amount);
        throw std::invalid_argument("Power consumption amount must be non-negative.");
    }
    battery_ = std::max(0, battery_-amount);
    if (battery_ == 0) {
        status_ = RobotStatus::Unavailable;
    }
};

// Move to next floor in the path
void Robot::move_to_next_floor() {
    if (curr_path_.empty()) {
        spdlog::error("Robot {} attempted to move but the path is empty.", id_);
        throw std::runtime_error("Cannot move to the next floor because the path is empty.");
    }
    curr_ = curr_path_.front();
    curr_path_.pop();
}

// Start task by going down the queue
void Robot::start_task() {
    Robot::move_to_next_task();
}

// Robot breaks randomly
void Robot::break_robot() {
    // Simulate 1% chance of breaking
    if ((((double)rand()) / INT_MAX) < 0.0001) {
        spdlog::warn("Robot {} has broken. Status set to Unavailable.", id_);
        status_ = RobotStatus::Unavailable;
    }
}

// Check if capacity is 0
bool Robot::is_capacity_empty() {
    if (remaining_capacity_ == 0) {
        return true;
    }
    return false;
}
