#include "simulation/simulator.hpp"


// Constructor for simulator
Simulator::Simulator() : ticking_(false), clock_{0} {};

// Destructor for simulator
// Wait for the sim_thread_ to finish if we want to wait
Simulator::~Simulator() {
    // Stop the clock thread when the Simulator is destroyed    
    ticking_ = false;
}

// Simulate the entire simulation
void Simulator::simulate() {
    while (ticking_) {
        std::cout << clock_ << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        clock_++;

        simulate_robots();
        simulate_floors();

        // Report status every 5 ticks
        if (clock_ % 5 == 0) {
            for (Robot& robot : robots_) {
                notify("five_sec_ping", robot.to_string());
            }
        }

        // End of simulation
        if (clock_ >= MAX_SIM_TIME) {
            ticking_ = false;
        }
    }
    std::string data; // TODO: method to do this manual stuff
    for (Robot& robot : robots_) {
        data += robot.to_string() + "\n\u200B"; // TODO: create better fix for this
    }
    for (Floor& floor : floorplan_.get_all_floor()) {
        data += "\n\u200B" + floorplan_.floor_to_string(floor) + "\n\u200B";
    }

    data += "\n\u200B" + floorplan_.to_string();
    // Report the finished simulation result to an output file
    spdlog::info("Simulation finished!");
    notify("finished_ping", data);
}

// simulate the robots
void Simulator::simulate_robots() {
    // Drain power accordingly
    std::lock_guard<std::mutex> lock(robots_mutex_);
    for (Robot& robot : robots_) {
        robot.break_robot(); // Random robot breaking down
        switch (robot.get_status()) {
            case RobotStatus::Available:
                robot.consume_power();
                if (!robot.tasks_empty() && can_move(robot)) {
                    // Set the path to task
                    robot.set_curr_path(floorplan_.get_path(robot.get_curr(), robot.get_first_task()));
                    robot.start_task();
                } else {
                    if (robot.get_battery() < 50) {
                        // Set the path to base
                        robot.set_curr_path(floorplan_.get_path(robot.get_curr(), robot.get_base()));
                        robot.go_charge();
                    }; // Go back to charge if battery < 50
                }
                break;
            case RobotStatus::Charging:
                if (robot.at_base()) {
                    robot.charge(); // only charge if you reached base
                } else {
                    robot.move_to_next_floor();
                }
                break;
            case RobotStatus::Traveling:
                robot.consume_power(); // traveling power consumption
                robot.move_to_next_task();
                break;
            case RobotStatus::Cleaning:
                robot.consume_power(3); // Cleaning power consumption
                // Check if room is clean
                if (floorplan_.access_floor(robot.get_curr()).is_clean()) {
                    if (!robot.tasks_empty() && can_move(robot)) {
                        // Set the path to task
                        robot.set_curr_path(floorplan_.get_path(robot.get_curr(), robot.get_first_task()));
                        robot.move_to_next_task();
                    } else {
                        robot.set_status(RobotStatus::Available);
                    }
                }
                break;
            case RobotStatus::Unavailable:
                notify("finished_ping", "Needs Fixing: \n" + robot.to_string());
                break;
        }
    }
}

// simulate the floors
void Simulator::simulate_floors() {
    std::lock_guard<std::mutex> lock(robots_mutex_);
    for (Floor& floor: floorplan_.get_all_floor()) {
        if (floor.get_getting_clean()) {
            floor.clean(RobotSize::Small); // TODO: based on number of robots in that room
        } else {
            floor.dirty();
        }
    }

}

// Call on the simulate() to start simulation
void Simulator::start_simulation() {
    // Prevent starting the clock if it's already running
    if (!ticking_) {
        ticking_ = true;
        spdlog::info("Simulation started!");
        sim_thread_ = std::thread(&Simulator::simulate, this);
        sim_thread_.detach();
    } else {
        spdlog::warn("There is an ongoing simulation. Cannot start another simulation!");
    }
}

// Reset the simulation
void Simulator::reset_simulation() {
    if (ticking_) {
        ticking_ = false;
        spdlog::info("Simulation reset! Total time: {} ticks", clock_);
    } else {
        spdlog::warn("No simulation to reset!");
    }
    clock_ = 0;  // Reset clock
}


// Add robot to the vector of robots_
void Simulator::add_robot(int id, std::string name, RobotSize size, RobotType type, int base, int curr, RobotStatus status) {
    Robot robot(id, name, size, type, base, curr, status);
    std::lock_guard<std::mutex> lock(robots_mutex_);
    robots_.push_back(std::ref(robot)); // Pass in the reference of robot object to be able to manipulate them
}

// Add floor to the vector of floors_
void Simulator::add_floor(int id, std::string name, FloorRoomType room, FloorType floortype, FloorSize size, FloorInteraction interaction_level, bool restriction, int clean_level, std::vector<int> neighbors) {
    if (floorplan_.get_size() <= MAX_NUM_FLOORS) {
        Floor new_floor(id, name, room, floortype, size, interaction_level, restriction, clean_level);
        std::vector<Floor> neighbor_floors;
        for (const auto& floor : floorplan_.get_all_floor()) {
            if (std::find(neighbors.begin(), neighbors.end(), floor.get_id()) != neighbors.end()) {
                neighbor_floors.push_back(floor);
            }
        }
        floorplan_.add_floor(std::ref(new_floor), neighbor_floors);
    } else {
        spdlog::error("MAXIMUM NUMBER OF FLOORS REACHED! Current: {} floors", floorplan_.get_size());
    }
}
    

// Add task to a robot
// TODO: assign multiple rooms to a robot task queue
void Simulator::add_task(int robot_id, int floor_id) {
    std::lock_guard<std::mutex> lock(robots_mutex_);
    for (Robot& robot : robots_) {
        if (robot.get_id() == robot_id) {
            robot.add_tasks_to_back({floor_id});
        }
    }
}

// Return a status of a robot
std::string Simulator::status_report(int robot_id) {
    std::lock_guard<std::mutex> lock(robots_mutex_);
    for (Robot& robot : robots_) {
        if (robot.get_id() == robot_id) {
            return robot.to_string();
        }
    }
    return std::to_string(robot_id) + " not found.";
}

bool Simulator::can_move(Robot& robot) {
    int path_to_task_to_base_distance = floorplan_.get_path(robot.get_curr(), robot.get_first_task()).size()
                                        + floorplan_.get_path(robot.get_first_task(), robot.get_base()).size(); // Path length total from curr to task and task back to base
    if (path_to_task_to_base_distance > robot.get_battery()) {
        return false;
    } 
    return true;
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

// Return all floor names in string vector
std::vector<std::string> Simulator::get_all_floor_names() {
    std::vector<Floor> all_floors = floorplan_.get_all_floor();
    std::vector<std::string> floor_names;
    for (Floor floor : all_floors) {
        floor_names.push_back(floor.get_name());
    }
    return floor_names;
}