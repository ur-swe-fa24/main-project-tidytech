#include "simulation/robot.hpp"
#include "spdlog/spdlog.h"

Robot::Robot(std::string id, std::string size, std::string type, std::string base, std::string curr) {
    id_ = id;
    Robot::setSize(size);
    Robot::setType(type);
    base_ = base;
    curr_ = curr;
    battery_ = 100;
    status_ = Robot::Status::Available;
} 

void Robot::setSize(std::string size) {
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

void Robot::setType(std::string type) {
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

void Robot::setStatus(std::string status) {
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

std::string Robot::toString() {
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


void Robot::addTasksToBack(std::vector<std::string> floors) {
    task_queue_.insert(task_queue_.end(), floors.begin(), floors.end());
}

void Robot::addTasksToFront(std::vector<std::string> floors) {
    task_queue_.insert(task_queue_.begin(), floors.begin(), floors.end());
}

bool Robot::moveToNext() {
    if (Robot::canMove()) {
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

void Robot::charge() {
    if (curr_ == base_) {
        battery_ = std::min(100, battery_ + 5); // Charge 5 percentage every tick
        if(battery_==100){status_ = Robot::Status::Available;} 
    }
}

void Robot::goCharge() {
    /**
     * implement more with battery usage and moving robot
     */
    status_ = Robot::Status::Charging;
    curr_ = base_;
}

bool Robot::canMove() {
    /**
     * Future: Check if the next floor has max capacity
     * Future: Check if the robot can go back to base
     */
    if (battery_ > 5) {return true;}
    return false;
}

void Robot::startTask() {
    Robot::moveToNext();
}