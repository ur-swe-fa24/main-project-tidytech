#include "simulation/simulator.hpp"
#include "spdlog/spdlog.h"
#include <iostream> // For testing

Simulator::Simulator() : ticking_(true), sim_thread_(), clock_{0} {};

Simulator::~Simulator() {
    // Stop the clock thread when the Simulator is destroyed
    ticking_ = false;
    sim_thread_.join();
}

void Simulator::simulate() {
    while (ticking_) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        clock_++;

        // Drain power accordingly
        std::lock_guard<std::mutex> lock(robots_mutex_);
        for (Robot& robot : robots_) {
            switch (robot.getStatus()) {
                case Robot::Status::Available:
                    robot.consumePower();
                    if (!robot.tasksEmpty()){robot.startTask();} // Go do something
                    else {
                        if (robot.getBattery() < 50) {robot.goCharge();};
                    }
                    break;
                case Robot::Status::Charging:
                    robot.charge();
                    break;
                case Robot::Status::Cleaning:
                    robot.consumePower(3); // Cleaning takes more power
                    break;
                case Robot::Status::Unavailable:
                    break;
            }
        }

        // Report status every 5 ticks
        if (clock_ % 5 == 0) {
            for (Robot& robot : robots_) {
                robot.toString();
            }
        }

        if (clock_ >= Simulator::MAX_SIM_TIME) {
            ticking_ = false;
        }
    }
}

void Simulator::start_simulation() {
    // Prevent starting the clock if it's already running
    if (!ticking_) {
        ticking_ = true;
        sim_thread_ = std::thread(&Simulator::simulate, this);
        spdlog::info("Simulation started!");
    } else {
        spdlog::warn("There is an ongoing simulation. Cannot start another simulation!");
    }
}

void Simulator::reset_simulation() {
    if (ticking_) {
        ticking_ = false;
        sim_thread_.join();  // Wait for the thread to finish to join
        spdlog::info("Simulation reset! Total time: {} ticks", clock_);
        clock_ = 0;  // Reset clock
    } else {
        spdlog::warn("No simulation to reset!");
    }
}

void Simulator::add_robot(std::string id, std::string size, std::string type, std::string base, std::string curr) {
    Robot robot(id, size, type, base, curr);
    std::lock_guard<std::mutex> lock(robots_mutex_);
    robots_.push_back(std::ref(robot)); // Pass in the reference of robot object to be able to manipulate them
}

void Simulator::add_floor(std::string floor) {
    floors_.push_back(floor);
}

std::string Simulator::clean(std::string robot_id, std::string floor_id) {
    std::lock_guard<std::mutex> lock(robots_mutex_);
    for (Robot& robot : robots_) {
        if (robot.getId() == robot_id) {
            robot.addTasksToBack({floor_id});
            return floor_id + " added to " + robot_id + "'s queue.";
        }
    }
    return robot_id + " not found.";
}

std::string Simulator::status_report(std::string robot_id) {
    std::lock_guard<std::mutex> lock(robots_mutex_);
    for (Robot& robot : robots_) {
        if (robot.getId() == robot_id) {
            robot.toString();
        }
    }
    return robot_id + " not found.";
}

// Testing
// int main() {
//     Simulator sim1{};
//     sim1.add_robot("R1", "small", "vaccum", "base", "base");
//     sim1.add_robot("R2", "large", "shampoo", "base", "base");
//     std::cout << sim1.clean("R1", "Hallway1") << std::endl;
//     std::cout << sim1.clean("R1", "Hallway2") << std::endl;
//     sim1.start_simulation();
//     std::this_thread::sleep_for(std::chrono::seconds(10));
//     sim1.reset_simulation();
// }