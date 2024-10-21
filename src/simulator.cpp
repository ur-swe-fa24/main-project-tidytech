#include "simulation/simulator.hpp"


// Simulator::Simulator(Fleet_manager* fm) : fm_(fm), ticking_(false), clock_{0} {};
Simulator::Simulator() : ticking_(true), clock_{0} {};

Simulator::~Simulator() {
    // Stop the clock thread when the Simulator is destroyed
    ticking_ = false;
    if (sim_thread_.joinable()) {sim_thread_.join();}
}

void Simulator::simulate() {
    while (ticking_) {
        std::cout << clock_ << std::endl;
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
                notify("five_sec_ping", robot.toString());
            }
        }

        if (clock_ >= Simulator::MAX_SIM_TIME) {
            ticking_ = false;
        }
    }
    std::string data; // TODO: method to do this manual stuff
    for (Robot& robot : robots_) {
        data += robot.toString() + "\n\u200B"; // TODO: create better fix for this
    }
    notify("finished_ping", data);
}

void Simulator::start_simulation() {
    // Prevent starting the clock if it's already running
    simulate();
    // if (!ticking_) {
    //     ticking_ = true;
    //     sim_thread_ = std::thread(&Simulator::simulate, this);
    //     //spdlog::info("Simulation started!");
    // } else {
    //     //spdlog::warn("There is an ongoing simulation. Cannot start another simulation!");
    // }
}

// void Simulator::reset_simulation() {
//     if (ticking_) {
//         ticking_ = false;
//         sim_thread_.join();  // Wait for the thread to finish to join
//         //spdlog::info("Simulation reset! Total time: {} ticks", clock_);
//         clock_ = 0;  // Reset clock
//     } else {
//         //spdlog::warn("No simulation to reset!");
//     }
// }

void Simulator::add_robot(std::string id, std::string size, std::string type, std::string base, std::string curr) {
    Robot robot(id, size, type, base, curr);
    std::lock_guard<std::mutex> lock(robots_mutex_);
    robots_.push_back(std::ref(robot)); // Pass in the reference of robot object to be able to manipulate them
}

void Simulator::add_floor(std::string floor) {
    floors_.push_back(floor);
}

void Simulator::add_task(std::string robot_id, std::string floor_id) {
    std::lock_guard<std::mutex> lock(robots_mutex_);
    for (Robot& robot : robots_) {
        if (robot.getId() == robot_id) {
            robot.addTasksToBack({floor_id});
        }
    }
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

void Simulator::subscribe(Subscriber* subscriber, const std::string& event) {
    subscribers_[event].push_back(subscriber);
}

void Simulator::unsubscribe(Subscriber* subscriber, const std::string& event) {
    auto& subs = subscribers_[event];
    subs.erase(std::remove(subs.begin(), subs.end(), subscriber), subs.end());
}

void Simulator::notify(const std::string& event, const std::string& data) {
    for (auto& subscriber : subscribers_[event]) {
        subscriber->update(event, data);
    }
}