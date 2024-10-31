#include "simulation/simulator.hpp"


// Constructor for simulator
Simulator::Simulator() : ticking_(false), clock_{0} {};

// Destructor for simulator
// Wait for the sim_thread_ to finish if we want to wait
Simulator::~Simulator() {
    // Stop the clock thread when the Simulator is destroyed    
    if (sim_thread_.joinable()) {
        sim_thread_.join();
    }
    ticking_ = false;
}

// Simulate the entire simulation
// TODO: integrate in with sim_thread_
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
                        if (robot.getBattery() < 50) {robot.goCharge();}; // Go back to charge if battery < 50
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

        // End of simulation
        if (clock_ >= MAX_SIM_TIME) {
            ticking_ = false;
        }
    }
    std::string data; // TODO: method to do this manual stuff
    for (Robot& robot : robots_) {
        data += robot.toString() + "\n\u200B"; // TODO: create better fix for this
    }
    // Report the finished simulation result to an output file
    notify("finished_ping", data);
}

// Call on the simulate() to start simulation
// TODO: integrate threads
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

// Reset the simulation
void Simulator::reset_simulation() {
    if (ticking_) {
        ticking_ = false;
        sim_thread_.join();  // Wait for the thread to finish to join
        spdlog::info("Simulation reset! Total time: {} ticks", clock_);
    } else {
        spdlog::warn("No simulation to reset!");
    }
    clock_ = 0;  // Reset clock
}

// Add robot to the vector of robots_
void Simulator::add_robot(std::string id, std::string size, std::string type, std::string base, std::string curr) {
    Robot robot(id, size, type, base, curr);
    std::lock_guard<std::mutex> lock(robots_mutex_);
    robots_.push_back(std::ref(robot)); // Pass in the reference of robot object to be able to manipulate them
}

// Add floor to the vector of floors_
void Simulator::add_floor(std::string floor) {
    floors_.push_back(floor);
}

// Add task to a robot
// TODO: assign multiple rooms to a robot task queue
void Simulator::add_task(std::string robot_id, std::string floor_id) {
    std::lock_guard<std::mutex> lock(robots_mutex_);
    for (Robot& robot : robots_) {
        if (robot.getId() == robot_id) {
            robot.addTasksToBack({floor_id});
        }
    }
}

// Return a status of a robot
std::string Simulator::status_report(std::string robot_id) {
    std::lock_guard<std::mutex> lock(robots_mutex_);
    for (Robot& robot : robots_) {
        if (robot.getId() == robot_id) {
            return robot.toString();
        }
    }
    return robot_id + " not found.";
}

// Let a subscriber subscribe to an event
void Simulator::subscribe(Subscriber* subscriber, const std::string& event) {
    subscribers_[event].push_back(subscriber);
}

// Let the subscriber unsubscribe from an event
void Simulator::unsubscribe(Subscriber* subscriber, const std::string& event) {
    auto& subs = subscribers_[event];
    subs.erase(std::remove(subs.begin(), subs.end(), subscriber), subs.end());
}

// Notify all the subscribers
void Simulator::notify(const std::string& event, const std::string& data) {
    for (auto& subscriber : subscribers_[event]) {
        subscriber->update(event, data);
    }
}