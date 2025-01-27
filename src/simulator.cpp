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
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        clock_++;

        simulate_robots(); // Simulate all the robots
        simulate_floors(); // Simulate all the floors


        // Report status every 5 ticks
        if (clock_ % 1 == 0) {
            for (Robot& robot : robots_) {
                // std::cout << robot.to_string() << std::endl;
                this->notify(Event::FiveSecReport, robot.to_string());
            }

            for (Floor& floor : floorplan_.get_all_floor()) {
                this->notify(Event::FiveSecReportFloors, floor.get_id_and_clean_level());
            }
        }

        // End of simulation
        if (clock_ >= MAX_SIM_TIME) {
            ticking_ = false;
        }
    }
    spdlog::info("MAX TIME REACHED! Simulation finished!");
}

// simulate the robots
void Simulator::simulate_robots() {
    // Drain power accordingly
    std::lock_guard<std::mutex> lock(robots_mutex_);
    for (Robot& robot : robots_) {
        robot.break_robot(); // Random robot breaking down
        if (can_move(robot)) {
            switch (robot.get_status()) {
                case RobotStatus::Available:
                    robot.consume_power();
                    check_out_of_battery(robot.get_id(), robot.get_battery());
                    if (!robot.tasks_empty()) {
                        // Set the path to task
                        try {
                            robot.set_curr_path(floorplan_.get_path(robot.get_curr(), robot.get_first_task()));
                            robot.start_task();
                        } catch (const std::exception& e) {
                            std::cout << "Error: " << e.what() << std::endl;
                            robot.move_to_next_task();
                        }
                    } else {
                        if (robot.get_battery() < 50) {
                            try {
                                // Set the path to base
                                robot.set_curr_path(floorplan_.get_path(robot.get_curr(), robot.get_base()));
                                robot.go_charge();
                            } catch (const std::exception& e) {
                                std::cout << "Error: " << e.what() << std::endl;
                            }
                        }
                    }
                    update_robot_db(robot, 1);
                    break;
                case RobotStatus::Charging:
                    if (robot.at_base()) {
                        robot.charge(); // only charge if you reached base
                        update_robot_db(robot, 0);
                    } else {
                        // Keep moving to base
                        robot.consume_power();
                        check_out_of_battery(robot.get_id(), robot.get_battery());
                        robot.move_to_next_floor();
                        update_robot_db(robot, 1);
                    }
                    
                    break;
                case RobotStatus::Traveling:
                    robot.consume_power(); // traveling power consumption
                    check_out_of_battery(robot.get_id(), robot.get_battery());
                    robot.move_to_next_task();
                    update_robot_db(robot, 1);
                    break;
                case RobotStatus::Cleaning:
                    robot.consume_power(3); // Cleaning power consumption
                    check_out_of_battery(robot.get_id(), robot.get_battery());
                    robot.clean(); // lower capacity
                    if (robot.is_capacity_empty()) {
                        // Set the path to base
                        try {
                            if (!floorplan_.access_floor(robot.get_curr()).is_clean()) {
                                robot.add_tasks_to_front({robot.get_curr()}); // attach the current room to the front of the task if floor is not cleaned when you need to reset capacity
                            }
                            robot.set_curr_path(floorplan_.get_path(robot.get_curr(), robot.get_base()));
                            robot.go_empty();
                            update_robot_db(robot, 3);
                        } catch (const std::exception& e) {
                            std::cout << "Error: " << e.what() << std::endl;
                        }
                        
                        break;
                    }
                    // Check if room is clean
                    if (floorplan_.access_floor(robot.get_curr()).is_clean()) {
                        notify(Event::UpdateNumFloorsClean, robot.get_id());
                        if (!robot.tasks_empty()) {
                            try {
                                // Set the path to task
                                robot.set_curr_path(floorplan_.get_path(robot.get_curr(), robot.get_first_task()));
                                robot.move_to_next_task();
                            } catch (const std::exception& e) {
                                std::cout << "Error: " << e.what() << std::endl;
                                robot.move_to_next_task();
                            }
                        } else {
                            robot.set_status(RobotStatus::Available);
                        }
                    }
                    update_robot_db(robot, 3);
                    break;
                case RobotStatus::NeedEmpty:
                    robot.consume_power();
                    check_out_of_battery(robot.get_id(), robot.get_battery());
                    if (robot.at_base()) {
                        notify(Event::AlertEmpty, robot.get_id());
                    } else {
                        robot.move_to_next_floor();
                    }
                    update_robot_db(robot, 1);
                    break;
                case RobotStatus::Unavailable:
                    update_robot_db(robot, 0);
                    notify(Event::UpdateRobotError, robot.get_id(), ErrorType::RandomBreak, false);
                    break;
            }
        } else {
            // Set status
            if (robot.get_status() != RobotStatus::Charging) {
                try {
                    // Set the path back to base to charge
                    robot.set_curr_path(floorplan_.get_path(robot.get_curr(), robot.get_base()));
                    robot.set_status(RobotStatus::Charging);
                } catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
                
            }
            // Go back and charge
            if (robot.at_base()) {
                robot.charge(); // only charge if you reached base
                update_robot_db(robot, 0);
            } else {
                robot.consume_power();
                check_out_of_battery(robot.get_id(), robot.get_battery());
                robot.move_to_next_floor();
                update_robot_db(robot, 1);
            }
        }
    }
}

// simulate the floors
void Simulator::simulate_floors() {
    std::lock_guard<std::mutex> lock(floors_mutex);
    for (Floor& floor: floorplan_.get_all_floor()) {
        floor.set_getting_clean(false);
        // Check which robots are cleaning this floor
        for (const Robot& robot : robots_) {
            if ((robot.get_curr() == floor.get_id()) && (robot.get_status()==RobotStatus::Cleaning)) {
                floor.set_getting_clean(true);
                floor.clean(robot.get_size());
                // Update the floor in floorplan
                floorplan_.update_floor(floor);
            }
        }
        // Get dirty if no robots are cleaning
        if (!floor.get_getting_clean()) {
            floor.dirty();
            // Update the floor in floorplan
            floorplan_.update_floor(floor);
        }
        notify(Event::UpdateFloorCleanLevel, floor.get_id(), floor.get_clean_level());
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
void Simulator::add_robot(int id, std::string name, RobotSize size, RobotType type, int base, int curr, RobotStatus status, int remaining_capacity, std::vector<int> task_queue, std::vector<int> path, int current_battery, int total_battery_used, int error_count, int rooms_cleaned) {
    Robot robot(id, name, size, type, base, curr, status, current_battery, remaining_capacity);
    std::lock_guard<std::mutex> lock(robots_mutex_);
    robots_.push_back(std::ref(robot)); // Pass in the reference of robot object to be able to manipulate them
}

// Update robot in db
void Simulator::update_robot_db(Robot& robot, int powerUsed) {
    // Update db
    vector<int> curr_path;
    queue<int> tmp_queue = robot.get_curr_path();
    while (!tmp_queue.empty()) {
        curr_path.push_back(tmp_queue.front());
        tmp_queue.pop();
    }
    notify(Event::UpdateRobotParameters, std::to_string(robot.get_id()), std::to_string(robot.get_curr()), to_string(robot.get_status()), std::to_string(robot.get_remaining_capacity()), robot.get_task_queue(), curr_path, robot.get_battery(), powerUsed); // Consume 1 power unit
}

// Add floor to the vector of floors_
void Simulator::add_floor(int id, std::string name, FloorRoomType room, FloorType floortype, FloorSize size, FloorInteraction interaction_level, bool restriction, int clean_level, std::vector<int> neighbors) {
    std::lock_guard<std::mutex> lock(floors_mutex);
    if (floorplan_.get_all_floor().size() <= MAX_NUM_FLOORS) {
        Floor new_floor(id, name, room, floortype, size, interaction_level, restriction, clean_level);
        std::vector<Floor> neighbor_floors;
        for (const auto& floor : floorplan_.get_all_floor()) {
            // Push all the floor neighbors
            if (std::find(neighbors.begin(), neighbors.end(), floor.get_id()) != neighbors.end()) {
                neighbor_floors.push_back(floor);
            }
        }

        try {
            floorplan_.add_floor(std::ref(new_floor), neighbor_floors); // Add the floor to floorplan
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            floorplan_.update_floor_neighbors(std::ref(new_floor), neighbor_floors); // Update the floor neighbors
        }

        // Update database push
        for (const Floor& floor : neighbor_floors) {
            std::vector<Floor> neighbors = floorplan_.get_neighbors(floor);
            std::vector<int> neighbors_ids;
            for (const Floor& floor : neighbors) {
                neighbors_ids.push_back(floor.get_id());
            }
            notify(Event::UpdateFloorNeighbors, floor.get_id(), neighbors_ids);
        }
    } else {
        spdlog::error("MAXIMUM NUMBER OF FLOORS REACHED! Current: {} floors", floorplan_.get_all_floor().size());
    }
}
    

// Add tasks at the end
void Simulator::add_task_to_back(int robot_id, std::vector<int> floor_ids) {
    std::lock_guard<std::mutex> lock(robots_mutex_);
    for (Robot& robot : robots_) {
        if (robot.get_id() == robot_id) {
            robot.add_tasks_to_back(filter_tasks(robot, floor_ids));
            update_robot_db(robot, 0);
            return;
        }
    }
    throw std::runtime_error("Robot not found in Simulator");
}

// Add tasks at the front
void Simulator::add_task_to_front(int robot_id, std::vector<int> floor_ids) {
    std::lock_guard<std::mutex> lock(robots_mutex_);
    for (Robot& robot : robots_) {
        if (robot.get_id() == robot_id) {
            robot.add_tasks_to_front(filter_tasks(robot, floor_ids));
            update_robot_db(robot, 0);
            return;
        }
    }
    spdlog::error("Robot {} is not in Simulator", robot_id);
    throw std::runtime_error("Robot not found in Simulator");
}

// Filter tasks so that you cannot add tasks that the robot cannot reach
vector<int> Simulator::filter_tasks(Robot& robot, vector<int> tasks) {

    vector<int> filtered1;
    vector<int> filtered2;
    RobotType robot_type = robot.get_type();
    int curr = robot.get_curr();

    auto all_floors = floorplan_.get_all_floor();

    // Removes all unreachable tasks
    for (const int t : tasks) {
        try {
            floorplan_.get_path(curr, t);
            filtered1.push_back(t);
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }

    // Removes all unmatched types
    for (const int t : filtered1) {
        for (auto floor : all_floors) {
            if (t == floor.get_id() && check_robot_to_floor(robot_type, floor.get_floortype())) {
                filtered2.push_back(t);
            }
        }
    }

    return filtered2;
}

// Fix all unavailable robots
void Simulator::resolve_all_robots() {
    std::lock_guard<std::mutex> lock(robots_mutex_);
    for (Robot& robot : robots_) {
        if (robot.get_status() == RobotStatus::Unavailable) {
            robot.fix_error();
            update_robot_db(robot, 0);
            notify(Event::UpdateRobotError, robot.get_id(), true);
        }
    }
}

// Reset capacity of all robots
void Simulator::reset_capacity_all_robots() {
    std::lock_guard<std::mutex> lock(robots_mutex_);
    for (Robot& robot : robots_) {
        if ((robot.get_status() == RobotStatus::NeedEmpty) && (robot.at_base())) {
            robot.reset_capacity();
            (robot, 0);
        }
    }
}

// Check if robot battery is out
void Simulator::check_out_of_battery(int id, int battery) {
    if (battery < 1) {
        notify(Event::UpdateRobotError, id, ErrorType::OutOfBattery, false);
    }
}

// Check RobotType and Floortype
bool Simulator::check_robot_to_floor(RobotType robot_type, FloorType floor_type) {
    switch(robot_type) {
        case RobotType::Scrubber:
            switch (floor_type) {
                case FloorType::Wood:
                    return true;
                case FloorType::Tile:
                    return true;
                case FloorType::Carpet:
                    return false;
            }
        case RobotType::Shampoo:
            switch (floor_type) {
                case FloorType::Wood:
                    return false;
                case FloorType::Tile:
                    return false;
                case FloorType::Carpet:
                    return true;
            }
        case RobotType::Vaccum:
            return true;
    }
}

// Access the robot
Robot Simulator::get_robot(int robot_id) {
    std::lock_guard<std::mutex> lock(robots_mutex_);
    for (Robot& robot : robots_) {
        if (robot.get_id() == robot_id) {
            return robot;
        }
    }
    spdlog::error("Robot {} is not in Simulator", robot_id);
    throw std::invalid_argument("Robot not found in Simulator");
}

// Access the floor
Floor Simulator::get_floor(int floor_id) {
    std::lock_guard<std::mutex> lock(floors_mutex);
    for (Floor& floor : floorplan_.get_all_floor()) {
        if (floor.get_id() == floor_id) {
            return floor;
        }
    }
    spdlog::error("Floor {} is not in Simulator", floor_id);
    throw std::invalid_argument("Floor not found in Simulator");
}

// Get the neighbors of a given floor_id
std::vector<Floor> Simulator::get_neighbors(int floor_id) {
    std::lock_guard<std::mutex> lock(floors_mutex);
    auto floors = floorplan_.get_all_floor();
    for (const Floor floor : floors) {
        if (floor.get_id() == floor_id) {
            return floorplan_.get_neighbors(floor);
        }
    }
    spdlog::error("Floor {} is not in Simulator", floor_id);
    throw std::invalid_argument("Floor not found in Simulator");
}


// Return a status of a robot
std::string Simulator::status_report(int robot_id) {
    for (Robot& robot : robots_) {
        if (robot.get_id() == robot_id) {
            return robot.to_string();
        }
    }
    return std::to_string(robot_id) + " not found.";
}

// Check if the robot can move
bool Simulator::can_move(Robot& robot) {
    try {
        int path_base_distance = floorplan_.get_path(robot.get_curr(), robot.get_base()).size() + 3; // +3 to account for cleaning this current room
        if (path_base_distance < robot.get_battery()) {
            return true;
        }
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    
    if (robot.get_status() == RobotStatus::Cleaning) {
        robot.add_tasks_to_front({robot.get_curr()}); // attach the current room to the front of the task
    }

    return false;
}


// Let a subscriber subscribe to an event
void Simulator::subscribe(Subscriber* subscriber, const Event& event) {
    subscribers_[event].push_back(subscriber);
}

// Let the subscriber unsubscribe from an event
void Simulator::unsubscribe(Subscriber* subscriber, const Event& event) {
    auto& subs = subscribers_[event];
    subs.erase(std::remove(subs.begin(), subs.end(), subscriber), subs.end());
}

// Notify all the subscribers
void Simulator::notify(const Event& event, const std::string& data) {
    for (auto& subscriber : subscribers_[event]) {
        subscriber->update(event, data);
    }
}

// Notify all the subscribers
void Simulator::notify(const Event& event, const int id, const int val) {
    for (auto& subscriber : subscribers_[event]) {
        subscriber->update(event, id, val);
    }
}

// Notify all the subscribers
void Simulator::notify(const Event& event, const int id, const vector<int>& data) {
    for (auto& subscriber : subscribers_[event]) {
        subscriber->update(event, id, data);
    }
}

void Simulator::notify(const types::Event& event, const int id) {
    for (auto& subscriber : subscribers_[event]) {
        subscriber->update(event, id);
    }
}

void Simulator::notify(const types::Event& event, const int id, const ErrorType error_type, const bool resolved) {
    for (auto& subscriber : subscribers_[event]) {
        subscriber->update(event, id, error_type, resolved);
    }
}

// Notify all the subscribers
void Simulator::notify(const types::Event& event, const std::string& id, const std::string& currentLocation, const std::string& status, const std::string& capacity, 
                    const std::vector<int>& taskQueue, const std::vector<int>& path, const int& currentBattery, const int& totalBatteryUsed) {
    for (auto& subscriber : subscribers_[event]) {
        subscriber->update(event, id, currentLocation, status, capacity, taskQueue, path, currentBattery, totalBatteryUsed);
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

// Return all robot names in string vector
std::vector<std::string> Simulator::get_all_robot_names() {
    std::vector<std::string> robot_names;
    for (auto robot : robots_) {
        robot_names.push_back(robot.get_name());
    }
    return robot_names;
}