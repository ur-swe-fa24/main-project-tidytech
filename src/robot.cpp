#include "simulation/robot.hpp"
#include "spdlog/spdlog.h"

// Robot constructor
// Initialize all variables
// TODO: Implement floorNode for base and curr
Robot::Robot(int id, std::string name, RobotSize size, RobotType type, std::string base, std::string curr, RobotStatus status) {
    id_ = id;
    name_ = name;
    size_ = size;
    type_ = type;
    base_ = base;
    curr_ = curr;
    battery_ = 100;
    status_ = status;
} 

// to_string for robot object
std::string Robot::to_string() const {
    std::string str_tasks = "";

    for (std::string task : task_queue_) {
        str_tasks += task + ", ";
    }

    if (str_tasks.length() >= 2) {
        str_tasks.erase(str_tasks.length() - 2, 2); // Remove last two characters
    }

    return "Robot Id: " + std::to_string(id_) +
            ", Name: " + name_ + "\n" +
            "Size: " + types::to_string(size_) +
            ", Type: " + types::to_string(type_) + "\n" +
            "Status: " + types::to_string(status_) +
            ", Battery: " + std::to_string(battery_) + "\n" +
            "Base Location: " + base_ + "\n"
            "Current Location: " + curr_ + "\n"
            "Tasks: " + str_tasks + "\n";
}

// Add tasks at the back of the queue
void Robot::add_tasks_to_back(std::vector<std::string> floors) {
    task_queue_.insert(task_queue_.end(), floors.begin(), floors.end());
}

// Add tasks at the front of the queue
void Robot::add_tasks_to_front(std::vector<std::string> floors) {
    task_queue_.insert(task_queue_.begin(), floors.begin(), floors.end());
}

// Move to next task if it can
// TODO: modify the logic to flow better
bool Robot::move_to_next() {
    if (Robot::can_move()) {
        if (!task_queue_.empty()) {
            // Pop the first task and move robot to that floor
            curr_ = task_queue_.front();
            status_ = RobotStatus::Cleaning;
            // task_queue_.erase(task_queue_.begin()); //Remove the first task TODO: make this work again
            return true;
        } else {
            status_ = RobotStatus::Available;
            spdlog::warn("No more tasks! Robot {} did not move", id_);
            return false;
        }
    } else {
        spdlog::warn("{} cannot move!", id_);
        return false;
    }
}

// Charge 5 unit power every call
// Must be at base
void Robot::charge() {
    if (curr_ == base_) {
        battery_ = std::min(100, battery_ + 5); // Charge 5 percentage every tick
        if(battery_==100){status_ = RobotStatus::Available;} 
    }
}

// Move to base and change status to Charginf
void Robot::go_charge() {
    /**
     * implement more with battery usage and moving robot
     */
    status_ = RobotStatus::Charging;
    curr_ = base_;
}

// Check if robot has enough battery to move
bool Robot::can_move() {
    /**
     * Future: Check if the next floor has max capacity
     * Future: Check if the robot can go back to base
     */
    if (battery_ > 5) {return true;}
    return false;
}

// Start task by going down the queue
void Robot::start_task() {
    Robot::move_to_next();
}