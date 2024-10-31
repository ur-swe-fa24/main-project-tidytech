#include "simulation/robot.hpp"
#include "spdlog/spdlog.h"

// Robot constructor
// Initialize all variables
// TODO: Implement floorNode for base and curr
Robot::Robot(std::string id, std::string size, std::string type, std::string base, std::string curr) {
    id_ = id;
    Robot::set_size(size);
    Robot::set_type(type);
    base_ = base;
    curr_ = curr;
    battery_ = 100;
    status_ = Robot::Status::Available;
} 

// Set size with string
void Robot::set_size(const std::string size) {
    if (size == "small") {
        size_ = Robot::Size::Small;
    } else if (size == "medium") {
        size_ = Robot::Size::Medium;
    } else if (size == "large") {
        size_ = Robot::Size::Large;
    } else {
        // Invalid Size
        spdlog::error("{} is an invalid Robot size", size);
        return;
    }
}

// Set type with string
void Robot::set_type(const std::string type) {
    if (type == "scrubber") {
        type_ = Robot::Type::Scrubber;
    } else if (type == "vacuum") {
        type_ = Robot::Type::Vaccum;
    } else if (type == "shampoo") {
        type_ = Robot::Type::Shampoo;
    } else {
        // Invalid type
        spdlog::error("{} is an invalid Robot type", type);
        return;
    }
}

// Set status with string
void Robot::set_status(const std::string status) {
    if (status == "available") {
        status_ = Robot::Status::Available;
    } else if (status == "cleaning") {
        status_ = Robot::Status::Cleaning;
    } else if (status == "charging") {
        status_ = Robot::Status::Charging;
    } else if (status == "unavailable") {
        status_ = Robot::Status::Unavailable;
    } else {
        // Invalid status
        spdlog::error("{} is an invalid Robot status", status);
        return;
    }
}

// toString for robot object
std::string Robot::to_string() const {
    std::string str_size;
    std::string str_type;
    std::string str_status;
    std::string str_tasks = "";

    switch(size_) {
        case Robot::Size::Large:
            str_size = "Large";
            break;
        case Robot::Size::Medium:
            str_size = "Medium";
            break;
        case Robot::Size::Small:
            str_size = "Small";
            break;
    }

    switch(type_) {
        case Robot::Type::Scrubber:
            str_type = "Scrubber";
            break;
        case Robot::Type::Shampoo:
            str_type = "Shampoo";
            break;
        case Robot::Type::Vaccum:
            str_type = "Vaccum";
            break;
    }

    switch (status_) {
        case Robot::Status::Available:
            str_status = "Available";
            break;
        case Robot::Status::Cleaning:
            str_status = "Cleaning";
            break;
        case Robot::Status::Charging:
            str_status = "Charging";
            break;
        case Robot::Status::Unavailable:
            str_status = "Unavailable";
            break;
    }

    for (std::string task : task_queue_) {
        str_tasks += task + ", ";
    }

    if (str_tasks.length() >= 2) {
        str_tasks.erase(str_tasks.length() - 2, 2); // Remove last two characters
    }

    return "Robot Id: " + id_ +
            ", Size: " + str_size +
            ", Type: " + str_type + "\n" +
            "Status: " + str_status +
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
            status_ = Robot::Status::Cleaning;
            // task_queue_.erase(task_queue_.begin()); //Remove the first task TODO: make this work again
            return true;
        } else {
            status_ = Robot::Status::Available;
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
        if(battery_==100){status_ = Robot::Status::Available;} 
    }
}

// Move to base and change status to Charginf
void Robot::go_charge() {
    /**
     * implement more with battery usage and moving robot
     */
    status_ = Robot::Status::Charging;
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